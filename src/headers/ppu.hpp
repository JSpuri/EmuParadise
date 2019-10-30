#ifndef PPU_HPP
#define PPU_HPP

class AddressBus;
#include "processor.hpp"

class PPU : public Processor {

    public:
        PPU();
        void WriteTo(uint16_t addr, int8_t value) override;
        uint8_t ReadFrom(uint16_t addr) override;

        void WriteToRegister(uint16_t addr, int8_t value);
        uint8_t ReadFromRegister(uint16_t addr);

        void SetAddressBus(AddressBus *addr_bus);

    private:
        AddressBus *addr_bus;

        uint8_t last_write_to_reg;

        uint8_t PPUCTRL;
        uint8_t PPUMASK;
        uint8_t PPUSTATUS;

        uint8_t OAMADDR;
        uint8_t OAMDATA;

        uint8_t PPUSCROLL;
        uint8_t cam_position_x;
        bool write_to_cam_x;
        uint8_t cam_position_y;

        uint16_t PPUADDR;
        bool write_to_ppuaddr_lower;

        uint8_t PPUDATA;

        uint8_t OAMDMA;

        bool odd_frame;
        uint16_t palette;

        uint8_t PPUGenLatch;

};


#endif
