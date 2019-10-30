#include "headers/addressbus.hpp"
#include <iostream>

AddressBus::AddressBus(CPU *cpu, Memory *memory) {

    this->cpu = cpu;
    this->memory = memory;

}

// Analisa qual classe quer escrever e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
void AddressBus::WriteTo(Processor *processor, uint16_t address, uint8_t word) {

    if(dynamic_cast<CPU*>(processor)){
        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address%PRG_RAM_SIZE;
            this->memory->WriteCPURAM(address, word);
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
            value = this->memory->ReadCPURAM(address);
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;
        }
        else if(address >= PRG_ROM_1_BANK_START){
            if(address < PRG_ROM_2_BANK_START){
                value = this->memory->ReadPRGROM(address);
            }
            else {
                if(this->memory->size_PRG_ROM_in_16kb_units == 2){
                    value = this->memory->ReadPRGROM(address);
                }
                else{
                    address -= PRG_ROM_2_BANK_START;
                    value = this->memory->ReadPRGROM(address);
                }
            }
        }
    }

    return value;
}
