#ifndef OPCODES_H
#define OPCODES_H

#include "memory.hpp"
#include "cpu.hpp"

class Opcodes {
    Memory *memory;
    CPU *cpu;

    public:
        Opcodes(Memory *memory, CPU *cpu);
        int ADC(int mode);

};

#endif