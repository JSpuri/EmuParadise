#include "headers/memory.hpp"

#include <bits/stdc++.h>

// Constructor
Memory::Memory(char *nesfile) {

    // Verifica o byte que nos diz quantos PGR ROMs de 16kb ele tem
    // Se tem dois, tem 32kb de ROM
    bool has32kbPRG_ROM = ((nesfile[4] == 2) ? true : false);
    
    // E se tem 32kb de ROM, devemos dobrar o tamanho padrao
    uint16_t rom_size =  PRG_ROM_SIZE * ((has32kbPRG_ROM) ? 2 : 1);

    // Reserva a memoria e inicializa tudo com zero
    this->PRG_RAM.resize(PRG_RAM_SIZE, 0);
    this->PRG_ROM.resize(rom_size, 0);

    // Guarda a informacao que tem dois bancos de 16kb de PGR ROM
    this->has_32kb_PRG_ROM = has32kbPRG_ROM;
    
    // Loop de copia do arquivo binario para o vetor de uint8_t (de fato nossa memoria)
    for(uint16_t i = 0; i < rom_size; i++)
        this->PRG_ROM[i] = nesfile[i + 0x10];

    // Guarda os enderecos de NMI, RESET e IRQ para facilitar no futuro
    this->NMI_ADDR = this->PRG_ROM[(rom_size - UPR_NMI_ADDR)] << 8;
    this->NMI_ADDR += this->PRG_ROM[(rom_size - LWR_NMI_ADDR)];

    this->RESET_ADDR = this->PRG_ROM[(rom_size - UPR_RST_ADDR)] << 8;
    this->RESET_ADDR += this->PRG_ROM[(rom_size - LWR_RST_ADDR)];

    this->IRQ_ADDR = this->PRG_ROM[(rom_size - UPR_IRQ_ADDR)] << 8;
    this->IRQ_ADDR += this->PRG_ROM[(rom_size - LWR_IRQ_ADDR)];


    // Inicializa variavel que verifica se a memoria foi acessada
    this->was_accessed = false;
    this->last_accessed_mem = 0;

}

// Le um determinado endereço da memoria. Nao eh necessario um cast ao entrar um zero_pg_addr
uint8_t Memory::read(uint16_t addr) {

    this->last_accessed_mem = addr;

    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if (addr < 0x2000) {
        addr = addr%PRG_RAM_SIZE;
        this->last_accessed_mem = addr;
        return this->PRG_RAM[addr];
    }


    if (this->has_32kb_PRG_ROM) {
        this->last_accessed_mem = addr;
        return this->PRG_ROM[addr - PRG_ROM_1_BANK_START];
    }

    //if the cartridge doesn't have 32KB, we must mirror
    //$8000-$BFFF and $C000-$FFFF
    else{

        if (addr >= PRG_ROM_1_BANK_START && addr < PRG_ROM_2_BANK_START) {
            this->last_accessed_mem = addr;
            return this->PRG_ROM[addr - PRG_ROM_1_BANK_START];
        }

        else if (addr >= PRG_ROM_2_BANK_START && addr <= 0xFFFF) {
            this->last_accessed_mem = addr - PRG_ROM_SIZE;
            return this->PRG_ROM[addr - PRG_ROM_1_BANK_START - PRG_ROM_SIZE];
        }
    }

    return 0;

}

// Escreve um valor de 8 bits num endereço. Nao eh necessario um cast ao entrar um zero_pg_addr
void Memory::write(uint16_t addr, int8_t value) {

    this->was_accessed= true;
    this->last_accessed_mem = addr;
    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if(addr < 0x2000){

        addr = addr%PRG_RAM_SIZE;
        this->last_accessed_mem = addr;

        this->PRG_RAM[addr] = value;
    }
    
}

