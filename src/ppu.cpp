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
void PPU::Rendering(unsigned int NumCPUCycles) {

    if(NumCPUCycles < 2250 && this->gen_nmi_at_vblank_interval)
        return;

    this->in_vblank = false;

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
            this->PPUGenLatch = this->PPUDATA;
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

void PPU::WriteToOAMDMA(uint8_t value) {

    this->OAMDMA = value;
}

void PPU::WriteToOAM(uint8_t addr, uint8_t value) {

    this->OAM[addr] = value;
}

uint8_t PPU::ReadFromOAM(uint8_t addr) {

    return this->OAM[addr];
}

