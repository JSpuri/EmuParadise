#ifndef CPU_HPP
#define CPU_HPP

#include "processor.hpp"
#include "addressbus.hpp"
#include <functional>

#define C 7
#define Z 6
#define I 5
#define D 4
#define Bb 3
#define V 1
#define N 0

class AddressBus;

class CPU : public Processor {

    public:
        CPU();

        int8_t x;
        int8_t y;
        int8_t a;
        uint8_t sp;     //stack pointer
        uint16_t pc;    //program counter
        uint8_t ps[8];  //p[NV-BDIZC]

        uint16_t last_accessed_mem;

        bool Clock();
        
        int8_t ResolveOPArgWord(int mode, uint16_t addr);
        uint16_t ResolveOPArgAddr(int mode, uint16_t addr);

        void WriteTo(uint16_t addr, int8_t value);
        uint8_t ReadFrom(uint16_t addr);

        void SetAddressBus(AddressBus *addr_bus);

        void setInstructionMode(int mode);
        void setOperation(std::function<void(int, CPU*)> operation);
        void setInstructionNumBytes(int bytes);
        void setInstructionNumCycles(int cycles);
        void setInstructionAccessedMemory(bool hasAccessedMemory);

        void setInstruction(uint8_t opcode);
        void runInstruction();

    private:
        AddressBus *addr_bus;

        int instructionMode;
        uint8_t instructionOpcode;
        uint8_t instructionNumBytes;
        uint8_t instructionNumCycles;
        bool instructionAccessedMemory;

        std::function<void(int, CPU *)>operation;

        int8_t ReadImmediate(uint16_t addr);

        int8_t ResolveZeroAddr(uint16_t addr);
        int8_t ResolveZeroAddrX(uint16_t addr);
        int8_t ResolveZeroAddrY(uint16_t addr);

        int8_t ReadRelative(uint16_t addr);

        int8_t ResolveAbsAddr(uint16_t addr);
        int8_t ResolveAbsAddrX(uint16_t addr);
        int8_t ResolveAbsAddrY(uint16_t addr);

        int8_t ResolveIndirectX(uint16_t addr);
        int8_t ResolveIndirectY(uint16_t addr);

        uint16_t ReadZeroAddr(uint16_t addr);
        uint16_t ReadAbsAddr(uint16_t addr);

        uint16_t ResolveIndirect(uint16_t addr);
        uint16_t ResolveIndirectAddrX(uint16_t addr);
        uint16_t ResolveIndirectAddrY(uint16_t addr);

        void log();
        void logls();
};


#endif
