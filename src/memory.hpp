#include <bits/stdc++.h>

// Informacoes no memory.cpp :)
class Memory {

    public:
        Memory(char *nesfile);       // Constructor
        int8_t read(uint16_t addr);
        void write(uint16_t addr, int8_t value);
        bool wasWritten();          // returns was_written

        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_written_mem;

    private:
        std::vector<uint8_t> PRG_RAM;
        std::vector<uint8_t> PRG_ROM;
        bool has_32kb_PRG_ROM;
        bool was_written;       //tell us if altered memory must be printed

};

