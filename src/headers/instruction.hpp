#ifndef OPCODES_H
#define OPCODES_H

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "cpu.hpp"
#include "memory.hpp"

// Instruction class:
// Stores operation mode - as defined in ./common/constants.hpp
// and address of corresponding function - declared in ./headers/operations.hpp
// It can only Run said operation.
class Instruction {

    public:
        Instruction(CPU *cpu, uint8_t opcode);
        void Run();

    private:
        CPU *cpu;

        int mode;
        uint8_t opcode;
        uint8_t num_bytes;
        uint8_t num_cycles;
        bool access_memory;

        std::function<void(int, CPU *)>operation;

        void log();
        void logls();

};

#endif
