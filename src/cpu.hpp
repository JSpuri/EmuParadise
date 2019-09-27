#include <bits/stdc++.h>

#define C 0
#define Z 1
#define I 2
#define D 3
#define B 4
#define V 6
#define N 7

// Informacoes na cpu.cpp :)
class CPU {

    public:
        CPU(uint16_t reset_addr);
        int8_t x;
        int8_t y;
        int8_t a;
        uint8_t sp;     //stack pointer
        uint16_t pc;    //program counter
        uint8_t ps[8];  //p[NV-BDIZC]

};
