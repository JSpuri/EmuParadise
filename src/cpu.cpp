#include "headers/cpu.hpp"

#include "headers/instruction.hpp"
#include "headers/addressbus.hpp"
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

    this->addr_bus = NULL;
    this->num_cycles = 0;

    this->last_accessed_mem = 0;
}

// If it finds a BRK, returns false to stop the main loop
bool CPU::ExecuteNextInstruction() {

    uint8_t curr_opcode = this->addr_bus->ReadFrom(this, this->pc);

    // If next instruction is a BRK, stop the program
    if (curr_opcode == 0x00)
        return false;

    Instruction curr_instruction(this, curr_opcode);
    
    curr_instruction.Run();

    return true;
}

// This method returns a word (8 bits) - the instruction argument,
// depending on the operation mode. If the argument is a address, a word will
// be retrieved from that address - or the indirect address, if applied.
int8_t CPU::ResolveOPArgWord(int mode, uint16_t addr) {

    int8_t value = 0;

    if(mode == M_IMMEDIATE)
        value = ReadImmediate(addr);

    else if(mode == M_ZERO_PAGE)
        value = ResolveZeroAddr(addr);

    else if(mode == M_ZERO_PAGE_X)
        value = ResolveZeroAddrX(addr);

    else if(mode == M_ZERO_PAGE_Y)
        value = ResolveZeroAddrY(addr);

    else if(mode == M_RELATIVE)
        value = ReadRelative(addr);

    else if(mode == M_ABSOLUTE)
        value = ResolveAbsAddr(addr);

    else if(mode == M_ABSOLUTE_X)
        value = ResolveAbsAddrX(addr);

    else if(mode == M_ABSOLUTE_Y)
        value = ResolveAbsAddrY(addr);

    else if(mode == M_INDEXED_INDIRECT)
        value = ResolveIndirectX(addr);

    else if(mode == M_INDIRECT_INDEXED)
        value = ResolveIndirectX(addr);

    return value;

}

// This method returns a address (16 bits) - the instruction argument,
// depending on the operation mode. If the mode is indirect, the address
// will be retrieved from the first address
uint16_t CPU::ResolveOPArgAddr(int mode, uint16_t addr) {

    uint16_t value = 0;

    if(mode == M_ZERO_PAGE)
        value = ReadZeroAddr(addr);

    else if(mode == M_ZERO_PAGE_X)
        value = (ReadZeroAddr(addr) + this->x) & 0xFF;

    else if(mode == M_ZERO_PAGE_Y)
        value = ReadZeroAddr(addr) + this->y;

    else if(mode == M_ABSOLUTE)
        value = ReadAbsAddr(addr);

    else if(mode == M_ABSOLUTE_X)
        value = ReadAbsAddr(addr) + this->x;

    else if(mode == M_ABSOLUTE_Y)
        value = ReadAbsAddr(addr) + this->y;

    else if(mode == M_INDIRECT)
        value = ResolveIndirect(addr);
    
    else if(mode == M_INDEXED_INDIRECT)
        value = ResolveIndirectX(addr);

    else if(mode == M_INDIRECT_INDEXED)
        value = ResolveIndirectY(addr);
    
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

void CPU::SetAddressBus(AddressBus *addr_bus){

    this->addr_bus = addr_bus;
}

// Reads a word (8 bit) from the address, to be used in ResolveOPArgWord
// to retrieve immediate arguments in instructions
int8_t CPU::ReadImmediate(uint16_t addr) {

    int8_t immediate = this->addr_bus->ReadFrom(this, addr);
    return immediate;
}

uint16_t CPU::ReadZeroAddr(uint16_t addr) {

    uint16_t address = this->addr_bus->ReadFrom(this, addr);
    return address;
}

// Reads a address (16 bit) from a address, to be used in ResolveOPArgAddr
// to retrieve an absolute address argument from instructions
uint16_t CPU::ReadAbsAddr(uint16_t addr) {

    uint16_t address = this->addr_bus->ReadFrom(this, addr);
    address += this->addr_bus->ReadFrom(this, addr + 1) << 8;
    return address;
}

// Retrieves a word from a zero page address
int8_t CPU::ResolveZeroAddr(uint16_t addr) {

    uint8_t zero_pg_addr = this->addr_bus->ReadFrom(this, addr);

    int8_t value = this->addr_bus->ReadFrom(this, zero_pg_addr);
    return value;
}

// Retrieves a word from a zero page address with x offset
int8_t CPU::ResolveZeroAddrX(uint16_t addr) {

    uint8_t zero_pg_addr = this->addr_bus->ReadFrom(this, addr);

    int8_t value = this->addr_bus->ReadFrom(this, zero_pg_addr + this->x);
    return value;
}

// Retrieves a word from a zero page address with y offset
int8_t CPU::ResolveZeroAddrY(uint16_t addr) {

    uint8_t zero_pg_addr = this->addr_bus->ReadFrom(this, addr);

    int8_t value = this->addr_bus->ReadFrom(this, zero_pg_addr + this->y);
    return value;
}

// Reads a word (8 bit) from the address, to be used in ResolveOPArgWord
// to retrieve relative arguments in instructions
int8_t CPU::ReadRelative(uint16_t addr) {

    int8_t immediate = this->addr_bus->ReadFrom(this, addr);
    return immediate;
}

// Retrieves a word from an absolute address
int8_t CPU::ResolveAbsAddr(uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(addr);

    int8_t value = this->addr_bus->ReadFrom(this, absolute_addr);
    return value;
}

// Retrieves a word from an absolute address with x offset
int8_t CPU::ResolveAbsAddrX(uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(addr);

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->x) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = this->addr_bus->ReadFrom(this, absolute_addr + this->x);
    return value;
}

// Retrieves a word from an absolute address with y offset
int8_t CPU::ResolveAbsAddrY(uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(addr);

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->y) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = this->addr_bus->ReadFrom(this, absolute_addr + this->y);
    return value;
}

// Retrieves an address from an absolute addres
uint16_t CPU::ResolveIndirect(uint16_t addr) {

    uint16_t absolute_addr = ReadAbsAddr(addr);

    uint16_t value = this->addr_bus->ReadFrom(this, absolute_addr);
    value += this->addr_bus->ReadFrom(this, absolute_addr + 1) << 8;
    return value;
}

// Retrieves a word from an indirect address with x offset
int8_t CPU::ResolveIndirectX(uint16_t addr) {

    uint8_t zero_pg_addr = this->addr_bus->ReadFrom(this, addr);
    zero_pg_addr += this->x;

    uint16_t absolute_addr = ReadAbsAddr(zero_pg_addr);

    int8_t value = this->addr_bus->ReadFrom(this, absolute_addr);
    return value;
}

// Retrieves a word from an indirect address with y offset
int8_t CPU::ResolveIndirectY(uint16_t addr) {

    uint8_t zero_pg_addr = this->addr_bus->ReadFrom(this, addr);

    uint16_t absolute_addr = ReadAbsAddr(zero_pg_addr);
    absolute_addr += this->y;

    // Page crossing with absolute addresses with offsets adds one to cpu cycle
    if(((absolute_addr + this->y) & 0xFF00) != (absolute_addr & 0xFF00))
        this->IncrementNumCycles();

    int8_t value = this->addr_bus->ReadFrom(this, absolute_addr);
    return value;
}

