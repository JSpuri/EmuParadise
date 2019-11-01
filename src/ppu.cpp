#include "headers/ppu.hpp"

#include "headers/addressbus.hpp"

#include <iostream>

// PPU class constructor
// All information regarding variables are in headers/ppu.hpp
PPU::PPU() {

    this->addr_bus = NULL;

    this->OAM.resize(OAM_SIZE, 0);
    
    this->PPUCTRL = 0;
    this->base_nametable_addr = 0x2000;
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

    this->PPUADDR = 0;
    this->write_to_ppuaddr_lower = false;

    this->PPUDATA = 0;

    this->OAMDMA = 0;

    this->odd_frame = false;
    this->palette = 0x3f00;

    this->PPUGenLatch = 0;
}

// PPU Rendering
void PPU::Rendering(unsigned int NumCPUCycles, uint8_t p_matrix[SCREEN_SIZE_X][SCREEN_SIZE_Y]) {

    if(NumCPUCycles < 2250 && this->gen_nmi_at_vblank_interval)
        return;

    this->in_vblank = false;

    if(this->show_background)
        this->load_background(p_matrix);

    this->in_vblank = true;
    if(this->gen_nmi_at_vblank_interval)
        this->addr_bus->GenNMI();
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
            this->WriteToPPUCTRL(value);
            break;

        case PPUMASK_ADDR:
            this->WriteToPPUMASK(value);
            break;

        case OAMADDR_ADDR:
            this->OAMADDR = value;
            break;

        case OAMDATA_ADDR:
            this->WriteToOAMDATA(value);
            break;

        // Set pixel on the PPUSCROLL position
        // to be on the top left corner of rendered screen
        case PPUSCROLL_ADDR:
            this->WriteToPPUSCROLL(value);
            break;

        case PPUADDR_ADDR:
            this->WriteToPPUADDR(value);
            break;


        // Not sure what to do
        case PPUDATA_ADDR:
            this->WriteToPPUDATA(value);
            break;

        // Not sure what to do
        case OAMDMA_ADDR:
            this->WriteToOAMDMA(value);
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
            this->PPUGenLatch = this->ReadFromPPUSTATUS();
            break;

        case OAMDATA_ADDR:
            this->PPUGenLatch = this->ReadFromOAMDATA();
            break;

        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUGenLatch = this->ReadFromPPUDATA();
            break;
    }

    return this->PPUGenLatch;
}

// Set which address it will read/write from/to
void PPU::SetAddressBus(AddressBus *addr_bus) {

    this->addr_bus = addr_bus;
}

// Write value to PPUCTRL and updates its internal variables for easier use
void PPU::WriteToPPUCTRL(uint8_t value) {

    this->PPUCTRL = value;

    this->base_nametable_addr = 0x2000 + ((this->PPUCTRL & 0x03) * 0x0400);
    this->vram_increment_addr_across = ((this->PPUCTRL & 0x04) ? 32 : 1);

    this->sprite_pattern_table_addr = ((this->PPUCTRL & 0x08) ? 0x1000 : 0x000);
    this->background_pattern_table_addr = ((this->PPUCTRL & 0x10) ? 0x1000 : 0x000);

    this->sprite_size = ((this->PPUCTRL & 0x20) ? std::make_pair(8, 16) : std::make_pair(8, 8));
    this->ppu_master_slave_select = ((this->PPUCTRL & 0x40) ? true : false);
    this->gen_nmi_at_vblank_interval = ((this->PPUCTRL & 0x80) ? true : false);

}

// Write value to PPUMASK and updates its internal variables for easier use
void PPU::WriteToPPUMASK(uint8_t value) {

    this->PPUMASK = value;

    this->display_greyscale = ((this->PPUMASK & 0x01) ? true : false);

    this->show_background_in_leftmost_8pixels = ((this->PPUMASK & 0x02) ? true : false);
    this->show_sprites_in_leftmost_8pixels = ((this->PPUMASK & 0x04) ? true : false);

    this->show_background = ((this->PPUMASK & 0x08) ? true : false);
    this->show_sprites = ((this->PPUMASK & 0x10) ? true : false);

    this->emphasize_red = ((this->PPUMASK & 0x20) ? true : false);
    this->emphasize_green = ((this->PPUMASK & 0x40) ? true : false);
    this->emphasize_blue = ((this->PPUMASK & 0x80) ? true : false);
}

// Read value from its internal variables for easier use and updates PPUSTATUS
uint8_t PPU::ReadFromPPUSTATUS() {

    // Clear least 5 bits and add last_write_to_reg
    this->PPUSTATUS &= 0xE0;
    this->PPUSTATUS += this->last_write_to_reg;

    this->PPUSTATUS |= ((this->sprite_overflow) ? 0x20 : 0x00);
    this->PPUSTATUS |= ((this->sprite_zero_hit) ? 0x40 : 0x00);
    this->PPUSTATUS |= ((this->in_vblank) ? 0x80 : 0x00);

    // Is cleared everytime PPUSTATUS is read
    this->in_vblank = false;

    return this->PPUSTATUS;
}

void PPU::WriteToOAMDATA(uint8_t value) {

    this->OAMDATA = value;
    this->WriteToOAM(this->OAMADDR, this->OAMDATA);
    this->OAMADDR += 1;

}

uint8_t PPU::ReadFromOAMDATA() {

    this->OAMDATA = this->ReadFromOAM(this->OAMADDR);
    return this->OAMDATA;
}

// Kind of wrong. PPUSCROLL and PPUADDR should be "the same variable"
void PPU::WriteToPPUSCROLL(uint8_t value) {

    this->PPUSCROLL = value;

    if(this->write_to_cam_x){

        this->cam_position_x = this->PPUSCROLL;
        this->write_to_cam_x = false;
    }

    else{

        this->cam_position_y = this->PPUSCROLL;
        this->write_to_cam_x = true;
    }

}

void PPU::WriteToPPUADDR(uint8_t value) {

    if(this->write_to_ppuaddr_lower){

        this->PPUADDR &= 0xFF00;
        this->PPUADDR += value;
        this->write_to_ppuaddr_lower = false;
    }
    else{

        this->PPUADDR &= 0x00FF;
        // Valid addresses only between 0x0000 and 0x3FFF
        value %= PPUADDR_WRITE_ADDR_LIMIT;
        this->PPUADDR += value << 8;
        this->write_to_ppuaddr_lower = true;
    }

}

void PPU::WriteToPPUDATA(uint8_t value) {

    this->WriteTo(this->PPUADDR, value);
    this->PPUADDR += this->vram_increment_addr_across;
}

uint8_t PPU::ReadFromPPUDATA() {

    uint8_t value = this->ReadFrom(this->PPUADDR);
    this->PPUADDR += this->vram_increment_addr_across;

    return value;
}

void PPU::WriteToOAMDMA(uint8_t value) {

    this->OAMDMA = value;
}

void PPU::WriteToOAM(uint8_t addr, uint8_t value) {

    this->OAM[addr] = value;
}

uint8_t PPU::ReadFromOAM(uint8_t addr) {

    return this->OAM[addr];
}

// Godspeed to you, reading this
void PPU::load_background(uint8_t p_matrix[SCREEN_SIZE_X][SCREEN_SIZE_Y]) {

    uint8_t tile_number;
    uint8_t pallet_number;
    uint16_t pallet_att_index = 0x23C0;
    uint8_t row = 0;

    uint8_t tile[8][8] = {0};

    for(uint16_t i = 0; i < 0x400 ; i++){

        //http://wiki.nesdev.com/w/index.php/PPU_nametables
        // Pega o numero da tile no chr
        tile_number = this->ReadFrom(this->base_nametable_addr + i);
        printf("Tile number: %u\n", tile_number);

        //http://wiki.nesdev.com/w/index.php/PPU_attribute_tables
        // Pega o numero da paleta
        // Isso esta errado, mas bola pra frente. Nosso jogo eh preto e branco mesmo
        pallet_number = this->ReadFrom(pallet_att_index);
        // Ele guarda as cores de quatro tiles numa so
        if(i % 2 == 0){
            if(((this->base_nametable_addr + i) & 0xFFF0) % 0x40 != 0){
                // Top left
                pallet_number = pallet_number & 0x03;
            }
            else{
                // Bottom left
                pallet_number = (pallet_number & 0x30) >> 4;
            }
        }
        else if(i % 2 != 0){
            if(((this->base_nametable_addr + i) & 0xFFF0) % 0x40 != 0){
                // Top right
                pallet_number = (pallet_number & 0x0C) >> 2;
            }
            else{
                // Bottom right
                pallet_number = (pallet_number & 0xC0) >> 6;
            }
        }

        printf("Pallet number: %u\n", pallet_number);
        uint8_t color_1 = this->ReadFrom(PALLETE_RAM_INDEXES_START + 1 + (pallet_number * 0x04));
        uint8_t color_2 = this->ReadFrom(PALLETE_RAM_INDEXES_START + 2 + (pallet_number * 0x04));
        uint8_t color_3 = this->ReadFrom(PALLETE_RAM_INDEXES_START + 3 + (pallet_number * 0x04));

        printf("Color number 1: %u\n", color_1);
        printf("Color number 2: %u\n", color_2);
        printf("Color number 3: %u\n", color_3);

        //http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
        // agora vamos de fato carregar o sprite bg
        for(uint8_t j = 0; j <= 0x07; j++){

            printf("j: %u\n", j);
            uint8_t pixel_pattern_low = this->ReadFrom(this->background_pattern_table_addr + (0x10 * tile_number) + j);
            uint8_t pixel_pattern_high = this->ReadFrom(this->background_pattern_table_addr + (0x10 * tile_number) + j + 0x08);

            tile[j][7] = pixel_pattern_low & 0x01;
            tile[j][7] += (pixel_pattern_high & 0x01) << 1;

            if(tile[j][7] == 1)
                tile[j][7] = color_1;
            else if(tile[j][7] == 2)
                tile[j][7] = color_2;
            else if(tile[j][7] == 3)
                tile[j][7] = color_3;
            else
                tile[j][7] = 0x0f;

            tile[j][6] = pixel_pattern_low & 0x02;
            tile[j][6] += (pixel_pattern_high & 0x02) << 1;

            if(tile[j][6] == 1)
                tile[j][6] = color_1;
            else if(tile[j][6] == 2)
                tile[j][6] = color_2;
            else if(tile[j][6] == 3)
                tile[j][6] = color_3;
            else
                tile[j][6] = 0x0f;

            tile[j][5] = pixel_pattern_low & 0x04;
            tile[j][5] += (pixel_pattern_high & 0x04) << 1;

            if(tile[j][5] == 1)
                tile[j][5] = color_1;
            else if(tile[j][5] == 2)
                tile[j][5] = color_2;
            else if(tile[j][5] == 3)
                tile[j][5] = color_3;
            else
                tile[j][5] = 0x0f;

            tile[j][4] = pixel_pattern_low & 0x08;
            tile[j][4] += (pixel_pattern_high & 0x08) << 1;

            if(tile[j][4] == 1)
                tile[j][4] = color_1;
            else if(tile[j][4] == 2)
                tile[j][4] = color_2;
            else if(tile[j][4] == 3)
                tile[j][4] = color_3;
            else
                tile[j][4] = 0x0f;

            tile[j][3] = pixel_pattern_low & 0x10;
            tile[j][3] += (pixel_pattern_high & 0x10) << 1;

            if(tile[j][3] == 1)
                tile[j][3] = color_1;
            else if(tile[j][3] == 2)
                tile[j][3] = color_2;
            else if(tile[j][3] == 3)
                tile[j][3] = color_3;
            else
                tile[j][3] = 0x0f;

            tile[j][2] = pixel_pattern_low & 0x20;
            tile[j][2] += (pixel_pattern_high & 0x20) << 1;

            if(tile[j][2] == 1)
                tile[j][2] = color_1;
            else if(tile[j][2] == 2)
                tile[j][2] = color_2;
            else if(tile[j][2] == 3)
                tile[j][2] = color_3;
            else
                tile[j][2] = 0x0f;

            tile[j][1] = pixel_pattern_low & 0x40;
            tile[j][1] += (pixel_pattern_high & 0x40) << 1;

            if(tile[j][1] == 1)
                tile[j][1] = color_1;
            else if(tile[j][1] == 2)
                tile[j][1] = color_2;
            else if(tile[j][1] == 3)
                tile[j][1] = color_3;
            else
                tile[j][1] = 0x0f;

            tile[j][0] = pixel_pattern_low & 0x80;
            tile[j][0] += (pixel_pattern_high & 0x80) << 1;

            if(tile[j][0] == 1)
                tile[j][0] = color_1;
            else if(tile[j][0] == 2)
                tile[j][0] = color_2;
            else if(tile[j][0] == 3)
                tile[j][0] = color_3;
            else
                tile[j][0] = 0x0f;

        }

        for(uint8_t j = row * 0x7; j < (row*0x7) + 0x8; j++)
            for(uint8_t k = i%0x20; k < (i%0x20) + 0x8; k++)
                p_matrix[j][k] = tile[j - (row*0x8)][k - (i%0x20)];


        if(i % 0x20 == 0)
            row++;
        printf("New Row: %u\n", row);
    }

}

