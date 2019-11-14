#include "headers/ppu.hpp"

#include "headers/addressbus.hpp"

#include <iostream>

const uint32_t colors[16 * 4] = { 0xFF545454, 0xFF001E74, 0xFF081090, 0xFF300088, 0xFF440064, 0xFF5C0030, 0xFF540400, 0xFF3C1800, 0xFF202A00, 0xFF083A00, 0xFF004000, 0xFF003C00, 0xFF00323C, 0xFF000000, 0xFF000000, 0xFF000000,
								0xFF989698, 0xFF084BC4, 0xFF3032EC, 0xFF5C1EE4, 0xFF8814B0, 0xFFA01464, 0xFF982220, 0xFF783C00, 0xFF545A00, 0xFF287200, 0xFF087C00, 0xFF007628, 0xFF006678, 0xFF000000, 0xFF000000, 0xFF000000,
								0xFFECEEEC, 0xFF4C9AEC, 0xFF787CEC, 0xFFB062EC, 0xFFE454EC, 0xFFEC58B4, 0xFFEC6A64, 0xFFD48820, 0xFFA0AA00, 0xFF74C400, 0xFF4CD020, 0xFF38CC6C, 0xFF38B4CC, 0xFF3C3C3C, 0xFF000000, 0xFF000000,
								0xFFECEEEC, 0xFFA8CCEC, 0xFFBCBCEC, 0xFFD4B2EC, 0xFFECAEEC, 0xFFECAED4, 0xFFECB4B0, 0xFFE4C490, 0xFFCCD278, 0xFFB4DE78, 0xFFA8E290, 0xFF98E2B4, 0xFFA0D6E4, 0xFFA0A2A0, 0xFF000000, 0xFF000000
};

// PPU class constructor
// All information regarding variables are in headers/ppu.hpp
PPU::PPU() {

    this->nmi = false;

    this->addr_bus = NULL;

    this->scanline = -1;
    this->cycle = 0;

    this->next_bg_tile_id = 0;
    this->next_bg_tile_att = 0;

    this->next_bg_tile_lower = 0;
    this->next_bg_tile_higher = 0;

    this->vector_bg_pat_lower_bit = 0;
    this->vector_bg_pat_higher_bit = 0;

    this->vector_bg_att_lower_bit = 0;
    this->vector_bg_att_higher_bit = 0;

    this->OAM.resize(OAM_SIZE, 0);
    
    this->PPUCTRL = 0;
    this->nametable_x_offset = 0;
    this->nametable_y_offset = 0;
    this->vram_increment_addr_across = 1;
    this->sprite_pattern_table_addr = 0x0000;
    this->background_pattern_table_addr = 0x0000;
    this->sprite_size = std::make_pair(8, 8);
    this->ppu_master_slave_select = false;
    this->gen_nmi_at_vblank_interval = false;

    this->PPUMASK = 0;
    this->display_greyscale = false;
    this->show_background_in_leftmost_8pixels = false;
    this->show_sprites_in_leftmost_8pixels = false;
    this->show_background = false;
    this->show_sprites = false;
    this->emphasize_red = false;
    this->emphasize_green = false;
    this->emphasize_blue = false;

    this->PPUSTATUS = 0;
    this->last_write_to_reg = 0;
    this->sprite_overflow = false;
    this->sprite_zero_hit = false;
    this->in_vblank = false;

    this->OAMADDR = 0;
    this->OAMDATA = 0;

    this->PPUSCROLL = 0;
    this->write_to_cam_x = true;
    this->vram_addr.reg = 0;
    this->tram_addr.reg = 0;
    this->fine_cam_position_x = 0;

    this->address_latch = false;

    this->PPUDATA = 0;

    this->OAMDMA = 0;

    this->odd_frame = false;
    this->palette = 0x3f00;

    this->PPUGenLatch = 0;
}

void PPU::Clock() {

    //printf("Beg\nCycle: %d\nScanline: %d\n", cycle, scanline);
    if(this->scanline >= -1 && this->scanline < 240){

        if(this->scanline == 0 && this->cycle == 0)
            this->cycle = 1;

        if(this->scanline == -1 && this->cycle == 1)
            this->in_vblank = false;

        if((this->cycle >= 2 && this->cycle < 258) || (this->cycle >= 321 && this->cycle < 338)){

            //printf("Terceiro if to primeiro if\n");
            // Update shifters
            if(this->show_background){

                this->vector_bg_pat_lower_bit <<= 1;
                this->vector_bg_pat_higher_bit <<= 1;

                this->vector_bg_att_lower_bit <<= 1;
                this->vector_bg_att_higher_bit <<= 1;
            }

            //printf("%d\n", (cycle - 1)%8);
            switch((this->cycle - 1) % 8){

                case 0:

                    this->vector_bg_pat_lower_bit = (this->vector_bg_pat_lower_bit & 0xFF00) | this->next_bg_tile_lower;
                    this->vector_bg_pat_higher_bit = (this->vector_bg_pat_higher_bit & 0xFF00) | this->next_bg_tile_higher;
                    //printf("vram_addr: %04x\n", vram_addr.reg);

                    this->next_bg_tile_id = this->ReadFrom(0x2000 | (this->vram_addr.reg & 0x0FFF));
                    break;

                case 2:
                    this->next_bg_tile_att = this->ReadFrom(0x23C0 | (vram_addr.nametable_y << 11)
                                                                      | (vram_addr.nametable_x << 10)
                                                                      | ((vram_addr.cam_position_y >> 2) << 3)
                                                                      | (vram_addr.cam_position_x >> 2));

                    if(vram_addr.cam_position_y & 0x02)
                        this->next_bg_tile_att >>= 4;
                    if(vram_addr.cam_position_x & 0x02)
                        this->next_bg_tile_att >>= 2;

                    this->next_bg_tile_att &= 0x03;

                    break;
                case 4:
                    this->next_bg_tile_lower = this->ReadFrom(this->background_pattern_table_addr
                                                                + (uint16_t)(this->next_bg_tile_id << 4)
                                                                + this->vram_addr.fine_cam_position_y);
                    break;
                case 6:
                    this->next_bg_tile_higher = this->ReadFrom(this->background_pattern_table_addr
                                                                + (uint16_t)(this->next_bg_tile_id << 4)
                                                                + this->vram_addr.fine_cam_position_y
                                                                + 8);
                    break;
                case 7:
                    if(this->show_background || this->show_sprites){

                        if(this->vram_addr.cam_position_x == 31){
                            this->vram_addr.cam_position_x = 0;
                            this->vram_addr.nametable_x = ~this->vram_addr.nametable_x;
                        }
                        else
                            this->vram_addr.cam_position_x++;

                    }
                    break;
            }
        }

        if(cycle == 256){

            if(this->show_background || this->show_sprites){

                if(this->vram_addr.fine_cam_position_y < 7)
                    this->vram_addr.fine_cam_position_y++;

                else{

                    this->vram_addr.fine_cam_position_y = 0;

                    if(this->vram_addr.cam_position_y == 29){
                        this->vram_addr.cam_position_y = 0;
                        this->vram_addr.nametable_y = ~this->vram_addr.nametable_y;
                    }

                    else if(this->vram_addr.cam_position_y == 31)
                        this->vram_addr.cam_position_y = 0;

                    else
                        this->vram_addr.cam_position_y++;
                }

            }
        }

        if(this->cycle == 257){

            this->vector_bg_pat_lower_bit = (this->vector_bg_pat_lower_bit & 0xFF00) | this->next_bg_tile_lower;
            this->vector_bg_pat_higher_bit = (this->vector_bg_pat_higher_bit & 0xFF00) | this->next_bg_tile_higher;

            if(this->show_background || this->show_sprites){
            
                this->vram_addr.nametable_x = this->tram_addr.nametable_x;
                this->vram_addr.cam_position_x = this->tram_addr.cam_position_x;
            }
        }

        if(this->cycle == 338 || cycle == 340)
            this->next_bg_tile_id = this->ReadFrom(0x2000 | (this->vram_addr.reg & 0x0FFF));

        if(this->scanline == -1 && this->cycle >= 280 && cycle < 305){

            this->vram_addr.fine_cam_position_y = tram_addr.fine_cam_position_y;
            this->vram_addr.nametable_y = this->tram_addr.nametable_y;
            this->vram_addr.cam_position_y = this->tram_addr.cam_position_y;
        }

    }

    if(this->scanline == 240){
    }

    if(this->scanline >= 241 && this->scanline < 261){
        
        if(this->scanline == 241 && this->cycle == 1){

            this->in_vblank = true;
            
            if(this->gen_nmi_at_vblank_interval)
                this->nmi = true;
        }
    }

    uint8_t bg_pixel = 0;
    uint8_t bg_palette = 0;

    if(this->show_background){

        uint16_t bit_mux = 0x8000 >> this->fine_cam_position_x;

        uint8_t p0_pixel = (this->vector_bg_pat_lower_bit & bit_mux) > 0;
        uint8_t p1_pixel = (this->vector_bg_pat_higher_bit & bit_mux) > 0;

        bg_pixel = p1_pixel << 1;
        bg_pixel |= p0_pixel;

        uint8_t bg_pal0 = (this->vector_bg_att_lower_bit & bit_mux) > 0; 
        uint8_t bg_pal1 = (this->vector_bg_att_higher_bit & bit_mux) > 0; 
        bg_palette = (bg_pal1 << 1) | bg_pal0;

    }

    //Set pixel here
    uint8_t pixel = this->ReadFrom(0x3F00 + (bg_palette << 2) + bg_pixel);

    //printf("Vou setar [%d][%d] com %02x\n", cycle, scanline, pixel);
    if((this->cycle >= 0 && this->cycle < 256) && (this->scanline >= 0 && this->scanline < 240))
        this->p_matrix[cycle + scanline * SCREEN_SIZE_X] = colors[pixel];

    this->cycle++;

    if(this->cycle >= 341){

        this->cycle = 0;
        this->scanline++;

        if(this->scanline >= 261){
            this->scanline = -1;
            this->frame_complete = true;
        }
    }

    //printf("End\nCycle: %d\nScanline: %d\n", cycle, scanline);
}


// Write value to memory according to PPU address table
void PPU::WriteTo(uint16_t addr, int8_t value) {
    this->addr_bus->WriteTo(this, addr, value);
}

// Read value from memory according to PPU address table
uint8_t PPU::ReadFrom(uint16_t addr) {
    return this->addr_bus->ReadFrom(this, addr);
}

// Write value to register (ppu register addresses are in common/constants.hpp)
void PPU::WriteToRegister(uint16_t addr, int8_t value) {

    this->PPUGenLatch = value;

    switch(addr){
        case PPUCTRL_ADDR:
            this->PPUCTRL = value;

            this->nametable_x_offset = value & 0x01;
            this->tram_addr.nametable_x = this->nametable_x_offset;

            this->nametable_y_offset = (value & 0x02) >> 1;
            this->tram_addr.nametable_y = this->nametable_y_offset;

            this->vram_increment_addr_across = ((value & 0x04) ? 0x20 : 0x01);

            this->sprite_pattern_table_addr = ((value & 0x08) ? 0x1000 : 0x0000);
            this->background_pattern_table_addr = ((value & 0x10) ? 0x1000 : 0x0000);

            this->sprite_size = ((value & 0x20) ? std::make_pair(8, 16) : std::make_pair(8, 8));

            this->ppu_master_slave_select = value & 0x40;
            this->gen_nmi_at_vblank_interval = value & 0x80;

            this->last_write_to_reg = value;
            break;

        case PPUMASK_ADDR:
            this->PPUMASK = value;

            this->display_greyscale = value & 0x01;

            this->show_background_in_leftmost_8pixels = value & 0x02;
            this->show_sprites_in_leftmost_8pixels = value & 0x04;

            this->show_background = value & 0x08;
            this->show_sprites = value & 0x10;

            this->emphasize_red = value & 0x20;
            this->emphasize_green = value & 0x40;
            this->emphasize_blue = value & 0x80;

            this->last_write_to_reg = value;
            break;

        case OAMADDR_ADDR:
            this->OAMADDR = value;

            this->last_write_to_reg = value;
            break;

        case OAMDATA_ADDR:
            this->OAMDATA = value;
            this->WriteToOAM( this->OAMDATA);

            this->last_write_to_reg = value;
            break;

        // Set pixel on the PPUSCROLL position
        // to be on the top left corner of rendered screen
        case PPUSCROLL_ADDR:
            this->PPUSCROLL = value;

            if(this->address_latch){

                this->tram_addr.fine_cam_position_y = this->PPUSCROLL & 0x07;
                this->tram_addr.cam_position_y = this->PPUSCROLL >> 3;
                this->address_latch = false;
            }

            else{

                this->fine_cam_position_x = this->PPUSCROLL & 0x07;
                this->tram_addr.cam_position_x = this->PPUSCROLL >> 3;
                this->address_latch = true;
            }
            break;

        case PPUADDR_ADDR:
            this->PPUSCROLL= value;
            if(this->address_latch){

                this->tram_addr.reg = (this->tram_addr.reg & 0xFF00) | value;
                this->vram_addr = this->tram_addr;
                this->address_latch = false;
            }
            else{

                this->tram_addr.reg = (uint16_t)((value & 0x3F) << 8) | (this->tram_addr.reg & 0x00FF);
                this->address_latch = true;
            }

            break;


        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUDATA = value;

            this->WriteTo(this->vram_addr.reg, value);
            this->vram_addr.reg += this->vram_increment_addr_across;
            break;

        case OAMDMA_ADDR:
            this->OAMDMA = value;
            break;

        default:
            return;
    }

    // PPUSTATUS stores the least sig 5 bits written to the ppu registers
    this->last_write_to_reg += value & 0x1F;
}

// Read value from register (ppu register adresses are in common/constants.hpp)
// PPUGenLatch is the carrier from PPU to CPU
uint8_t PPU::ReadFromRegister(uint16_t addr) {

    switch(addr){

        case PPUSTATUS_ADDR:
            this->PPUSTATUS = this->last_write_to_reg & 0x1F;
            this->PPUSTATUS |= ((this->sprite_overflow) ? 0x20 : 0x00);
            this->PPUSTATUS |= ((this->sprite_zero_hit) ? 0x40 : 0x00);
            this->PPUSTATUS |= ((this->in_vblank) ? 0x80 : 0x00);
            this->in_vblank = false;

            this->address_latch = false;

            this->PPUGenLatch = this->PPUSTATUS;
            break;

        case OAMDATA_ADDR:
            this->OAMDATA = this->ReadFromOAM();
            this->PPUGenLatch = this->OAMDATA;
            break;

        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUGenLatch = this->PPUDATA;
            this->PPUDATA = this->ReadFrom(this->vram_addr.reg);

            if(this->vram_addr.reg > 0x3F00)
                this->PPUGenLatch = this->PPUDATA;

            this->vram_addr.reg += this->vram_increment_addr_across;
            break;
    }

    return this->PPUGenLatch;
}

// Set which address it will read/write from/to
void PPU::SetAddressBus(AddressBus *addr_bus) {

    this->addr_bus = addr_bus;
}

uint32_t* PPU::GetPMatrix() {
    return this->p_matrix;
}

void PPU::WriteToOAM(uint8_t value) {

    this->OAM[this->OAMADDR] = value;
    this->OAMADDR++;
}

uint8_t PPU::ReadFromOAM() {

    return this->OAM[this->OAMADDR];
}

