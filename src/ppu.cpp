#include "headers/ppu.hpp"

#include "headers/addressbus.hpp"

#include <iostream>

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

    this->p_matrix = new std::vector<std::vector<uint8_t>>(SCREEN_SIZE_Y, std::vector<uint8_t>(SCREEN_SIZE_X, 0));

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
    this->cam_position_x = 0;
    this->write_to_cam_x = true;
    this->cam_position_y = 0;

    this->write_to_ppuscrollandaddr_lower = false;
    this->ppuaddr = 0x0;

    this->PPUDATA = 0;

    this->OAMDMA = 0;

    this->odd_frame = false;
    this->palette = 0x3f00;

    this->PPUGenLatch = 0;
}

void PPU::Clock() {

    if(this->scanline >= -1 && this->scanline < 240){

        if(this->scanline == 0 && this->cycle == 0)
            this->cycle = 1;

        if(this->scanline == -1 && this->cycle == 1)
            this->in_vblank = false;

        if((this->cycle >= 2 && this->cycle < 258) || (this->cycle >= 321 && this->cycle < 338)){

            if(this->show_background){

                this->vector_bg_pat_lower_bit <<= 1;
                this->vector_bg_pat_higher_bit <<= 1;

                this->vector_bg_att_lower_bit <<= 1;
                this->vector_bg_att_higher_bit <<= 1;
            }

            switch((this->cycle - 1) % 8){

                case 0:

                    this->vector_bg_pat_lower_bit = (this->vector_bg_pat_lower_bit & 0xFF00) | this->next_bg_tile_lower;
                    this->vector_bg_pat_higher_bit = (this->vector_bg_pat_higher_bit & 0xFF00) | this->next_bg_tile_higher;

                    this->next_bg_tile_id = this->ReadFrom(this->ppuaddr);
                    break;
                case 2:
                    break;
                case 4:
                    this->next_bg_tile_lower = this->ReadFrom(this->background_pattern_table_addr
                                                                + (uint16_t)(this->next_bg_tile_id << 4));
                    break;
                case 6:
                    this->next_bg_tile_higher = this->ReadFrom(this->background_pattern_table_addr
                                                                + (uint16_t)(this->next_bg_tile_id << 4));
                    break;
                case 7:
                    break;
            }
        }

    }

    if(this->scanline >= 241 && this->scanline < 261){
        
        if(this->scanline == 241 && this->cycle == 1){

            this->in_vblank = true;
            
            if(this->gen_nmi_at_vblank_interval)
                this->nmi = true;
        }
    }

    if(this->show_background){
    }

    //Set pixel here

    this->cycle++;

    if(this->cycle >= 341){

        this->cycle = 0;
        this->scanline++;

        if(this->scanline >= 261){
            this->scanline = -1;
        }
    }
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
            this->nametable_y_offset = (value & 0x02) >> 1;

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

            if(this->write_to_ppuscrollandaddr_lower){

                this->cam_position_y = this->PPUSCROLL;
                this->write_to_ppuscrollandaddr_lower = false;
            }

            else{

                this->cam_position_x = this->PPUSCROLL;
                this->write_to_ppuscrollandaddr_lower = true;
            }
            break;

        case PPUADDR_ADDR:
            this->PPUSCROLL= value;
            if(this->write_to_ppuscrollandaddr_lower){

                this->ppuaddr &= 0xFF00;
                this->ppuaddr += value;
                this->write_to_ppuscrollandaddr_lower = false;
            }
            else{

                this->ppuaddr &= 0x00FF;
                // Valid addresses only between 0x0000 and 0x3FFF
                this->ppuaddr += value << 8;
                this->write_to_ppuscrollandaddr_lower = true;
            }

            this->ppuaddr %= PPUADDR_WRITE_ADDR_LIMIT;
            break;


        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUDATA = value;
            this->WriteTo(this->ppuaddr, this->PPUDATA);

            this->ppuaddr += this->vram_increment_addr_across;
            this->ppuaddr %= PPUADDR_WRITE_ADDR_LIMIT;
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

            this->write_to_ppuscrollandaddr_lower = false;

            this->PPUGenLatch = this->PPUSTATUS;
            break;

        case OAMDATA_ADDR:
            this->OAMDATA = this->ReadFromOAM();
            this->PPUGenLatch = this->OAMDATA;
            break;

        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUGenLatch = this->PPUDATA;
            this->PPUDATA = this->ReadFrom(this->ppuaddr);

            if(this->ppuaddr > 0x3F00)
                this->PPUGenLatch = this->PPUDATA;

            this->ppuaddr += this->vram_increment_addr_across;
            this->ppuaddr %= PPUADDR_WRITE_ADDR_LIMIT;

            break;
    }

    return this->PPUGenLatch;
}

// Set which address it will read/write from/to
void PPU::SetAddressBus(AddressBus *addr_bus) {

    this->addr_bus = addr_bus;
}

void PPU::WriteToOAM(uint8_t value) {

    this->OAM[this->OAMADDR] = value;
    this->OAMADDR++;
}

uint8_t PPU::ReadFromOAM() {

    return this->OAM[this->OAMADDR];
}

