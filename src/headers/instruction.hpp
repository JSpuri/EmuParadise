#ifndef OPCODES_H
#define OPCODES_H

#include <bits/stdc++.h>

#include "cpu.hpp"
#include "memory.hpp"

class Instruction {

    public:
        Instruction(CPU *cpu, Memory *memory, uint8_t opcode);
        void Run();

    private:
        Memory *memory;
        CPU *cpu;
        int mode;
        std::function<void(int, CPU *, Memory *)>operation;

        void log();
        void logls();

};

#endif
