#include <bits/stdc++.h>

#define C 7
#define Z 6
#define I 5
#define D 4
#define B 3
#define V 1
#define N 0

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
