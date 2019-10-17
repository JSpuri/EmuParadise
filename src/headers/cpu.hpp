#ifndef CPU_H
#define CPU_H

#include <bits/stdc++.h>

#include "memory.hpp"

#define C 7
#define Z 6
#define I 5
#define D 4
#define B 3
#define V 1
#define N 0

class CPU {

    public:
        CPU(uint16_t reset_addr);

        int8_t x;
        int8_t y;
        int8_t a;
        uint8_t sp;     //stack pointer
        uint16_t pc;    //program counter
        uint8_t ps[8];  //p[NV-BDIZC]

        bool ExecuteNextInstruction(Memory *memory);
        int8_t ResolveOPArgWord(int mode, Memory *memory, uint16_t addr);
        uint16_t ResolveOPArgAddr(int mode, Memory *memory, uint16_t addr);

    private:
        int8_t ReadImmediate(Memory *memory, uint16_t addr);
        uint16_t ReadAbsAddr(Memory *memory, uint16_t addr);

        int8_t ResolveZeroAddr(Memory *memory, uint16_t addr);
        int8_t ResolveZeroAddrX(Memory *memory, uint16_t addr);
        int8_t ResolveZeroAddrY(Memory *memory, uint16_t addr);

        int8_t ResolveAbsAddr(Memory *memory, uint16_t addr);
        int8_t ResolveAbsAddrX(Memory *memory, uint16_t addr);
        int8_t ResolveAbsAddrY(Memory *memory, uint16_t addr);

        uint16_t ResolveIndirect(Memory *memory, uint16_t addr);
        int8_t ResolveIndirectX(Memory *memory, uint16_t addr);
        int8_t ResolveIndirectY(Memory *memory, uint16_t addr);

};


#endif
