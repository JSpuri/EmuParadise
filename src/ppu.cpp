#include "headers/ppu.hpp"

#include "headers/addressbus.hpp"

#include <iostream>

// PPU class constructor
PPU::PPU() {

    this->addr_bus = NULL;
    this->last_write_to_reg = 0;
    
    this->PPUCTRL = 0;
    this->PPUMASK = 0;
    this->PPUSTATUS = 0;

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

// Write value to memory according to PPU address table
void PPU::WriteTo(uint16_t addr, int8_t value) {
}

// Read value from memory according to PPU address table
uint8_t PPU::ReadFrom(uint16_t addr) {
    return 0;
}

// Write value to register (ppu register addresses are in common/constants.hpp)
void PPU::WriteToRegister(uint16_t addr, int8_t value) {

    this->PPUGenLatch = value;

    switch(addr){
        case PPUCTRL_ADDR:
            this->PPUCTRL = value;
            break;

        case PPUMASK_ADDR:
            this->PPUMASK = value;
            break;

        case OAMADDR_ADDR:
            this->OAMADDR = value;
            break;

        case OAMDATA_ADDR:
            this->OAMDATA = value;
            this->OAMADDR += 1;
            break;

        // Set pixel on the PPUSCROLL position
        // to be on the top left corner of rendered screen
        case PPUSCROLL_ADDR:
            this->PPUSCROLL = value;

            if(this->write_to_cam_x){

                this->cam_position_x = this->PPUSCROLL;
                this->write_to_cam_x = false;
            }

            else{

                this->cam_position_y = this->PPUSCROLL;
                this->write_to_cam_x = true;
            }

            break;

        case PPUADDR_ADDR:
            if(this->write_to_ppuaddr_lower){

                this->PPUADDR &= 0xFF00;
                this->PPUADDR += value;
                this->write_to_ppuaddr_lower = false;
            }
            else{

                this->PPUADDR &= 0x00FF;
                this->PPUADDR += value << 8;
                this->write_to_ppuaddr_lower = true;
            }

            break;


        // Not sure what to do
        case PPUDATA_ADDR:
            this->PPUDATA = value;
            break;

        // Not sure what to do
        case OAMDMA_ADDR:
            this->OAMDMA = value;

        default:
            return;
    }

    // PPUSTATUS stores the least sig 4 bits written to the ppu registers
    this->last_write_to_reg += value & 0x0F;
}

// Read value from register (ppu register adresses are in common/constants.hpp)
// PPUGenLatch is the carrier from PPU to CPU
uint8_t PPU::ReadFromRegister(uint16_t addr) {

    switch(addr){

        case PPUSTATUS_ADDR:
            // Change PPUSTATUS to a function?
            this->PPUGenLatch = this->PPUSTATUS;
            break;

        case OAMDATA_ADDR:
            this->PPUGenLatch = this->OAMDATA;
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

