#ifndef MEM_HPP
#define MEM_HPP

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "../common/constants.hpp"

// Classe memoria
class Memory {

    public:
        // Methods
        Memory(char *nesfile);       // Constructor

        uint8_t ReadCPURAM(uint16_t addr);
        uint8_t ReadPRGROM(uint16_t addr);

        void WriteCPURAM(uint16_t addr, int8_t value);

        // Attributes
        uint8_t size_PRG_ROM_in_16kb_units;
        uint8_t size_CHR_ROM_in_8kb_units;

        uint8_t size_PGR_RAM_in_8kb_units;
        uint8_t size_CHR_RAM_in_8kb_units;

        unsigned int mirroring_type;

        uint16_t mapper_number;

        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_accessed_mem;
        bool was_accessed;

    private:
        std::vector<uint8_t> INTERNAL_CPU_RAM;
        // Nametables
        std::vector<uint8_t> INTERNAL_PPU_RAM;

        std::vector<uint8_t> PRG_ROM;
        // Pattern Tables - Pode ser CHR_RAM
        std::vector<uint8_t> CHR_ROM;

        std::vector<uint8_t> PRG_RAM;
        // Pattern Tables - Pode ser CHR_ROM
        std::vector<uint8_t> CHR_RAM;

};

#endif
