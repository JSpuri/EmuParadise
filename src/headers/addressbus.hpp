#ifndef ADDRESS_BUS_HPP
#define ADDRESS_BUS_HPP

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "cpu.hpp"
#include "memory.hpp"

// Address Bus Class
// Handles reading and writing between devices
class AddressBus {

    public:
        AddressBus(CPU *cpu, Memory *memory);
        void WriteTo(Processor *processor, uint16_t address, uint8_t word);
        uint8_t ReadFrom(Processor *processor, uint16_t address);

    private:
        CPU *cpu;
        Memory *memory;
};

#endif
