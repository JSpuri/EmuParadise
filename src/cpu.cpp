#include <bits/stdc++.h>

#define C 7
#define Z 6
#define I 5
#define D 4
#define B 3
#define V 1
#define N 0

// Classe CPU - pode facilitar nossa vida no futuro ao precisarmos
// realizar uma integracao entre varios componentes
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

CPU::CPU(uint16_t reset_addr) {

    this->a = 0;
    this->x = 0;
    this->y = 0;

    this->sp = 0xFA;
    this->pc = reset_addr;

    this->ps[C] = 0;
    this->ps[Z] = 0;
    this->ps[I] = 1;
    this->ps[D] = 0;
    this->ps[B] = 1;
    this->ps[B-1] = 1;
    this->ps[V] = 0;
    this->ps[N] = 0;
}
