#include "headers/cpu.hpp"

#include <bits/stdc++.h>

#include "headers/instruction.hpp"
#include "common/constants.hpp"

CPU::CPU(uint16_t reset_addr) {

    this->a = 0;
    this->x = 0;
    this->y = 0;

    this->sp = 0xFD;
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

// If it finds a BRK, returns false to stop the main loop
bool CPU::ExecuteNextInstruction(Memory *memory) {

    uint8_t curr_opcode = memory->read(this->pc);

    if (curr_opcode == 0x00)
        return false;

    Instruction curr_instruction(this, memory, curr_opcode);
    
    curr_instruction.Run();

    return true;
}

int8_t CPU::ResolveOPArgWord(int mode, Memory *memory, uint16_t addr) {

    int8_t value = 0;

    if(mode == M_IMMEDIATE)
        value = ReadImmediate(memory, addr);

    else if(mode == M_ZERO_PAGE)
        value = ResolveZeroAddr(memory, addr);

    else if(mode == M_ZERO_PAGE_X)
        value = ResolveZeroAddrX(memory, addr);

    else if(mode == M_ZERO_PAGE_Y)
        value = ResolveZeroAddrY(memory, addr);

    else if(mode == M_RELATIVE)
        value = ReadImmediate(memory, addr);

    else if(mode == M_ABSOLUTE)
        value = ResolveAbsAddr(memory, addr);

    else if(mode == M_ABSOLUTE_X)
        value = ResolveAbsAddrX(memory, addr);

    else if(mode == M_ABSOLUTE_Y)
        value = ResolveAbsAddrY(memory, addr);

    else if(mode == M_INDEXED_INDIRECT)
        value = ResolveIndirectX(memory, addr);

    else if(mode == M_INDIRECT_INDEXED)
        value = ResolveIndirectX(memory, addr);

    return value;

}

uint16_t CPU::ResolveOPArgAddr(int mode, Memory *memory, uint16_t addr) {

    uint16_t value = 0;

    if(mode == M_ABSOLUTE)
        value = ReadAbsAddr(memory, addr);

    else if(mode == M_INDIRECT)
        value = ResolveIndirect(memory, addr);
    
    return value;
}

int8_t CPU::ReadImmediate(Memory *memory, uint16_t addr) {

    int8_t immediate = memory->read(addr);
    return immediate;
}

uint16_t CPU::ReadAbsAddr(Memory *memory, uint16_t addr) {

    int16_t address = memory->read(addr);
    address += memory->read(addr + 1) << 8;
    return address;
}

int8_t CPU::ResolveZeroAddr(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr);
    return value;
}

int8_t CPU::ResolveZeroAddrX(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr + this->x);
    return value;
}

int8_t CPU::ResolveZeroAddrY(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr + this->y);
    return value;
}

int8_t CPU::ResolveAbsAddr(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    int8_t value = memory->read(absolute_addr);
    return value;
}

int8_t CPU::ResolveAbsAddrX(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    int8_t value = memory->read(absolute_addr + this->x);
    return value;
}

int8_t CPU::ResolveAbsAddrY(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    int8_t value = memory->read(absolute_addr + this->y);
    return value;
}

uint16_t CPU::ResolveIndirect(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    uint16_t value = memory->read(absolute_addr);
    value += memory->read(absolute_addr + 1) << 8;
    return value;
}

int8_t CPU::ResolveIndirectX(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);
    zero_pg_addr += this->x;

    uint16_t absolute_addr = ReadAbsAddr(memory, zero_pg_addr);

    int8_t value = memory->read(absolute_addr);
    return value;
}

int8_t CPU::ResolveIndirectY(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    uint16_t absolute_addr = ReadAbsAddr(memory, zero_pg_addr);
    absolute_addr += this->y;

    int8_t value = memory->read(absolute_addr);
    return value;
}

