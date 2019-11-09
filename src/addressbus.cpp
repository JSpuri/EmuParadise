#include "headers/addressbus.hpp"
#include <iostream>

AddressBus::AddressBus(char *nesfile, CPU *cpu) {

    // Verifica o byte que nos diz quantos PRG ROMs de 16kb ele tem
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

    // Verifica o tamanho da PRG_RAM (em unidades de 8kb)
    this->size_PRG_RAM_in_8kb_units = nesfile[8];

    // Redimensiona todos os bancos do cartucho de acordo com o necessario
    uint16_t pgr_rom_size = PRG_ROM_SIZE * this->size_PRG_ROM_in_16kb_units;
    uint16_t chr_rom_size = CHR_ROM_SIZE * this->size_CHR_ROM_in_8kb_units;
    uint16_t pgr_ram_size = PRG_RAM_SIZE * this->size_PRG_RAM_in_8kb_units;
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

    this->cpu = cpu;

}

bool AddressBus::Clock() {

    this->run_emulation = cpu->Clock();

    return this->run_emulation;
}

// Analisa qual classe quer escrever e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
void AddressBus::WriteTo(Processor *processor, uint16_t address, uint8_t word) {

    if(dynamic_cast<CPU*>(processor)){
        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address%PRG_RAM_SIZE;
            this->INTERNAL_CPU_RAM[address] = word;
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;
        }
    }
}

// Analisa de qual classe ler e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
uint8_t AddressBus::ReadFrom(Processor *processor, uint16_t address) {

    uint8_t value = 0;


    if(dynamic_cast<CPU*>(processor)){

        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address%PRG_RAM_SIZE;
            value = this->INTERNAL_CPU_RAM[address];
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;
        }
        else if(address >= PRG_ROM_1_BANK_START){
            if(address < PRG_ROM_2_BANK_START){
                value = this->PRG_ROM[address];
            }
            else {
                if(this->size_PRG_ROM_in_16kb_units == 2){
                    value = this->PRG_ROM[address];
                }
                else{
                    address -= PRG_ROM_2_BANK_START;
                    value = this->PRG_ROM[address];
                }
            }
        }
    }

    return value;
}
