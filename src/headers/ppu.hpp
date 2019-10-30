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

        uint8_t PPUCTRL;
        // ============================ The variables underneath here are contained in PPUCTRL
        // $2000, $2400, $2800 or $2C00
        uint16_t base_nametable_addr;

        // add 1 to vram (going across)
        // add 32 to vram (going down)
        uint8_t vram_increment_addr_across;

        // $0000 or $1000
        // Ignored in 8x16 sprite mode
        uint16_t sprite_pattern_table_addr;
        
        // $0000 or $1000
        uint16_t background_pattern_table_addr;

        // Sprite size: 8x8 (8,8) or 8x16 (8,16) pixels
        std::pair<uint8_t, uint8_t> sprite_size;

        //PPU master/slave select
        // (0: read backdrop from EXT pins; 1: output color on EXT pins)
        bool ppu_master_slave_select;
        
        // Generate an NMI at vertical blanking interval
        bool gen_nmi_at_vblank_interval;
        // =============================================================== End of PPUCTRL vars

        uint8_t PPUMASK;
        // ============================ The variables underneath here are contained in PPUMASK
        bool display_greyscale;

        bool show_background_in_leftmost_8pixels;
        bool show_sprites_in_leftmost_8pixels;

        bool show_background;
        bool show_sprites;

        bool emphasize_red;
        bool emphasize_green;
        bool emphasize_blue;
        // =============================================================== End of PPUMASK vars
        
        uint8_t PPUSTATUS;
        // ========================== The variables underneath here are contained in PPUSTATUS
        // PPUSTATUS stores the least sig 4 bits written to the ppu registers
        uint8_t last_write_to_reg;

        bool sprite_overflow;
        bool sprite_zero_hit;

        // Vertical blank has started (0: not in vblank; 1: in vblank).
        // Set at dot 1 of line 241 (the line *after* the post-render
        // line); cleared after reading $2002 and at dot 1 of the
        // pre-render line.
        bool in_vblank;
        // ============================================================= End of PPUSTATUS vars

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


        void WriteToPPUCTRL(uint8_t value);
        void WriteToPPUMASK(uint8_t value);
        uint8_t ReadFromPPUSTATUS();

};


#endif
