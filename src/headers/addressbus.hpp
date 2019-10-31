#ifndef ADDRESS_BUS_HPP
#define ADDRESS_BUS_HPP

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "../common/constants.hpp"
#include "cpu.hpp"
#include "ppu.hpp"

class CPU;
class PPU;

// Address Bus Class
// Handles reading and writing between devices
class AddressBus {

    public:
        AddressBus(char *nesfile, CPU *cpu, PPU *ppu);

        bool Clock();

        void WriteTo(Processor *processor, uint16_t address, uint8_t word);
        uint8_t ReadFrom(Processor *processor, uint16_t address);

        uint32_t system_clock;

        uint8_t size_PRG_ROM_in_16kb_units;
        uint8_t size_CHR_ROM_in_8kb_units;

        uint8_t size_PRG_RAM_in_8kb_units;
        uint8_t size_CHR_RAM_in_8kb_units;

        // Determina o tipo de mirroring: 0 eh horizontal, 1 eh vertical
        unsigned int mirroring_type;

        uint16_t mapper_number;

        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_accessed_mem;
        bool was_accessed;

    private:
        CPU *cpu;
        PPU *ppu;

        bool run_emulation;

        std::vector<uint8_t> INTERNAL_CPU_RAM;
        // Nametables/VRAM
        std::vector<uint8_t> INTERNAL_PPU_RAM;

        std::vector<uint8_t> PRG_ROM;
        // Pattern Tables - Pode ser CHR_RAM
        std::vector<uint8_t> CHR_ROM;

        std::vector<uint8_t> PRG_RAM;
        // Pattern Tables - Pode ser CHR_ROM
        std::vector<uint8_t> CHR_RAM;
};

#endif
