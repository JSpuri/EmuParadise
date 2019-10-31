#include "headers/memory.hpp"

#include <iostream>
// Constructor
Memory::Memory(char *nesfile) {

    // Verifica o byte que nos diz quantos PGR ROMs de 16kb ele tem
    // Se tem dois, tem 32kb de ROM
    this->size_PRG_ROM_in_16kb_units = nesfile[4];

    // Verifica o byte que nos diz quantos CHR ROMs de 8kb ele tem
    // Se tem zero, usa CHR RAM
    this->size_CHR_ROM_in_8kb_units = nesfile[5];

    if(this->size_CHR_ROM_in_8kb_units)
        this->size_CHR_RAM_in_8kb_units = 0;
    
    // Determina o tipo de mirroring: 0 eh horizontal, 1 eh vertical
    this->mirroring_type = nesfile[6] & 0X01;

    // Verifica o numero mapper do cartucho
    this->mapper_number = (nesfile[7] & 0xF0) + (nesfile[6] >> 4);

    // Verifica o tamanho da PGR_RAM (em unidades de 8kb)
    this->size_PGR_RAM_in_8kb_units = nesfile[8];

    // Redimensiona todos os bancos do cartucho de acordo com o necessario
    uint16_t pgr_rom_size = PRG_ROM_SIZE * this->size_PRG_ROM_in_16kb_units;
    uint16_t chr_rom_size = CHR_ROM_SIZE * this->size_CHR_ROM_in_8kb_units;
    uint16_t pgr_ram_size = PRG_RAM_SIZE * this->size_PGR_RAM_in_8kb_units;
    uint16_t chr_ram_size = CHR_RAM_SIZE * this->size_CHR_RAM_in_8kb_units;

    // Reserva as memorias e inicializa tudo com zero
    this->INTERNAL_CPU_RAM.resize(INTERNAL_CPU_RAM_SIZE, 0);
    this->INTERNAL_PPU_RAM.resize(INTERNAL_PPU_RAM_SIZE, 0);
    this->PRG_ROM.resize(pgr_rom_size, 0);
    this->CHR_ROM.resize(chr_rom_size, 0);
    this->PRG_RAM.resize(pgr_ram_size, 0);
    this->CHR_RAM.resize(chr_ram_size, 0);

    // Loop de copia do arquivo binario para o vetor de uint8_t (de fato nossa memoria)
    for(uint16_t i = 0; i < pgr_rom_size; i++)
        this->PRG_ROM[i] = nesfile[i + 0x10];

    for(uint16_t i = 0; i < chr_rom_size; i++)
        this->CHR_ROM[i] = nesfile[i + (0x10 + pgr_rom_size)];

    // Guarda os enderecos de NMI, RESET e IRQ para facilitar no futuro
    this->NMI_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_NMI_ADDR)] << 8;
    this->NMI_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_NMI_ADDR)];

    this->RESET_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_RST_ADDR)] << 8;
    this->RESET_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_RST_ADDR)];

    this->IRQ_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_IRQ_ADDR)] << 8;
    this->IRQ_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_IRQ_ADDR)];


    // Inicializa variavel que verifica se a memoria foi acessada
    this->was_accessed = false;
    this->last_accessed_mem = 0;

}

// Le um determinado endereço da memoria RAM da CPU
uint8_t Memory::ReadCPURAM(uint16_t addr) {

    this->last_accessed_mem = addr;
    return this->INTERNAL_CPU_RAM[addr];
}

// Le um determinado endereço da memoria RAM da PPU
uint8_t Memory::ReadPPURAM(uint16_t addr) {

    return this->INTERNAL_PPU_RAM[addr];
}

// Le um determinado endereço da memoria PRG ROM
uint8_t Memory::ReadPRGROM(uint16_t addr) {

    this->last_accessed_mem = addr;
    return this->PRG_ROM[addr];
}

// Le um determinado endereço da memoria PRG ROM
uint8_t Memory::ReadCHRROM(uint16_t addr) {

    return this->CHR_ROM[addr];
}

// Escreve um determinado valor em um endereco da memoria RAM da CPU
void Memory::WriteCPURAM(uint16_t addr, int8_t value) {

    this->was_accessed= true;
    this->last_accessed_mem = addr;
    this->INTERNAL_CPU_RAM[addr] = value;
    
}

// Escreve um determinado valor em um endereco da memoria RAM da PPU
void Memory::WritePPURAM(uint16_t addr, int8_t value) {

    this->INTERNAL_PPU_RAM[addr] = value;
    
}

