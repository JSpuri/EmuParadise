#ifndef MEM_H
#define MEM_H

#include <bits/stdc++.h>

// Informacoes no memory.cpp :)
class Memory {

    public:
        // Methods
        Memory(char *nesfile);       // Constructor
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, int8_t value);

        // Attributes
        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_accessed_mem;
        bool was_accessed;

    private:
        std::vector<uint8_t> PRG_RAM;
        std::vector<uint8_t> PRG_ROM;
        bool has_32kb_PRG_ROM;

};

#endif