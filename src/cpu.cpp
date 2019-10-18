#include "headers/cpu.hpp"

#include <bits/stdc++.h>

#include "headers/instruction.hpp"
#include "common/constants.hpp"

// CPU class constructor
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

    this->num_cycles = 0;
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

// This method returns a word (8 bits) - the instruction argument,
// depending on the operation mode. If the argument is a address, a word will
// be retrieved from that address - or the indirect address, if applied.
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
        value = ReadRelative(memory, addr);

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

// This method returns a address (16 bits) - the instruction argument,
// depending on the operation mode. If the mode is indirect, the address
// will be retrieved from the first address
uint16_t CPU::ResolveOPArgAddr(int mode, Memory *memory, uint16_t addr) {

    uint16_t value = 0;

    if(mode == M_ABSOLUTE)
        value = ReadAbsAddr(memory, addr);

    else if(mode == M_INDIRECT)
        value = ResolveIndirect(memory, addr);
    
    return value;
}

// Increments CPU num_cycles
void CPU::IncrementNumCycles() {

    this->num_cycles += 1;
}
void CPU::IncrementNumCycles(uint8_t num) {

    this->num_cycles += num;
}

void CPU::ResetNumCycles() {

    this->num_cycles = 0;
}

// Reads a word (8 bit) from the address, to be used in ResolveOPArgWord
// to retrieve immediate arguments in instructions
int8_t CPU::ReadImmediate(Memory *memory, uint16_t addr) {

    int8_t immediate = memory->read(addr);
    return immediate;
}

// Reads a address (16 bit) from a address, to be used in ResolveOPArgAddr
// to retrieve an absolute address argument from instructions
uint16_t CPU::ReadAbsAddr(Memory *memory, uint16_t addr) {

    int16_t address = memory->read(addr);
    address += memory->read(addr + 1) << 8;
    return address;
}

// Retrieves a word from a zero page address
int8_t CPU::ResolveZeroAddr(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr);
    return value;
}

// Retrieves a word from a zero page address with x offset
int8_t CPU::ResolveZeroAddrX(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr + this->x);
    return value;
}

// Retrieves a word from a zero page address with y offset
int8_t CPU::ResolveZeroAddrY(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    int8_t value = memory->read(zero_pg_addr + this->y);
    return value;
}

// Reads a word (8 bit) from the address, to be used in ResolveOPArgWord
// to retrieve relative arguments in instructions
int8_t CPU::ReadRelative(Memory *memory, uint16_t addr) {

    int8_t immediate = memory->read(addr);
    return immediate;
}

// Retrieves a word from an absolute address
int8_t CPU::ResolveAbsAddr(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    int8_t value = memory->read(absolute_addr);
    return value;
}

// Retrieves a word from an absolute address with x offset
int8_t CPU::ResolveAbsAddrX(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->x) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = memory->read(absolute_addr + this->x);
    return value;
}

// Retrieves a word from an absolute address with y offset
int8_t CPU::ResolveAbsAddrY(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->y) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = memory->read(absolute_addr + this->y);
    return value;
}

// Retrieves an address from an absolute addres
uint16_t CPU::ResolveIndirect(Memory *memory, uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(memory, addr);

    uint16_t value = memory->read(absolute_addr);
    value += memory->read(absolute_addr + 1) << 8;
    return value;
}

// Retrieves a word from an indirect address with x offset
int8_t CPU::ResolveIndirectX(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);
    zero_pg_addr += this->x;

    uint16_t absolute_addr = ReadAbsAddr(memory, zero_pg_addr);

    int8_t value = memory->read(absolute_addr);
    return value;
}

// Retrieves a word from an indirect address with y offset
int8_t CPU::ResolveIndirectY(Memory *memory, uint16_t addr) {

    uint8_t zero_pg_addr = memory->read(addr);

    uint16_t absolute_addr = ReadAbsAddr(memory, zero_pg_addr);
    absolute_addr += this->y;

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->y) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = memory->read(absolute_addr);
    return value;
}

