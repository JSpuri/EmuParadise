#ifndef PPU_HPP
#define PPU_HPP

class AddressBus;
#include "processor.hpp"
#include "../common/constants.hpp"

#include <vector>

class PPU : public Processor {

    public:
        PPU();

        // Vertical blank has started (0: not in vblank; 1: in vblank).
        // Set at dot 1 of line 241 (the line *after* the post-render
        // line); cleared after reading $2002 and at dot 1 of the
        // pre-render line.
        bool in_vblank;
        bool nmi;

        void Clock();

        void WriteTo(uint16_t addr, uint8_t value) override;
        uint8_t ReadFrom(uint16_t addr) override;

        void WriteToRegister(uint16_t addr, uint8_t value);
        uint8_t ReadFromRegister(uint16_t addr);

        void SetAddressBus(AddressBus *addr_bus);

        uint32_t *GetPMatrix();

        // OAM stores sprite data (64 sprites with 4 bytes each)
        // Pos 0: Y Position (OAM_Y_POS)
        // Pos 1: Index (OAM_INDEX_N)
        // Pos 2: Attributes (OAM_ATTRIBUTES)
        // Pos 3: X Position (OAM_X_POS)
        //std::vector<uint8_t> OAM;
        uint8_t OAM[256];

    private:
        AddressBus *addr_bus;

        int16_t scanline;
        int16_t cycle;

        uint8_t next_bg_tile_id;
        uint8_t next_bg_tile_att;

        uint8_t next_bg_tile_lower;
        uint8_t next_bg_tile_higher;

        uint16_t vector_bg_pat_lower_bit;
        uint16_t vector_bg_pat_higher_bit;

        uint16_t vector_bg_att_lower_bit;
        uint16_t vector_bg_att_higher_bit;

        uint8_t vector_fg_pat_lower[8];
        uint8_t vector_fg_pat_higher[8];

        uint8_t sprite_scanline[8 * 4];
        uint8_t sprite_count;

        bool sprite_zero_hit_possible;
        bool sprite_zero_being_rendered;

        uint32_t p_matrix[SCREEN_SIZE_X * SCREEN_SIZE_Y] = {0};

        uint8_t PPUCTRL;
        // ============================ The variables underneath here are contained in PPUCTRL
        // Offset of nametable to $2000, $2400, $2800 or $2C00
        uint8_t nametable_x_offset;
        uint8_t nametable_y_offset;

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
        // ============================================================= End of PPUSTATUS vars

        uint8_t OAMADDR;
        uint8_t OAMDATA;

        uint8_t PPUSCROLL;
        bool write_to_cam_x;

        union loopy_reg {
            struct {

                uint16_t cam_position_x : 5;
                uint16_t cam_position_y : 5;
                uint16_t nametable_x : 1;
                uint16_t nametable_y : 1;
                uint16_t fine_cam_position_y :  3;
                uint16_t NOTUSED : 1;
            };

            uint16_t reg = 0x0000;
        };

        loopy_reg vram_addr;
        loopy_reg tram_addr;

        uint8_t fine_cam_position_x;

        bool address_latch;

        uint8_t PPUDATA;

        uint8_t OAMDMA;

        bool odd_frame;
        uint16_t palette;

        uint8_t PPUGenLatch;

        bool frame_complete;
};


#endif
