#include "headers/addressbus.hpp"
#include <iostream>

AddressBus::AddressBus(CPU *cpu, Memory *memory, PPU *ppu) {

    this->cpu = cpu;
    this->memory = memory;
    this->ppu = ppu;

}

// Analisa qual classe quer escrever e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
void AddressBus::WriteTo(Processor *processor, uint16_t address, uint8_t word) {

    if(dynamic_cast<CPU*>(processor)){
        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address%PRG_RAM_SIZE;
            this->cpu->last_accessed_mem = address;
            this->memory->WriteCPURAM(address, word);
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;

            // Apenas somo pra ficar bonitinho com os valores certos em common/constants.hpp
            address += PPU_REGISTERS_START;

            // Escrita de fato depende da PPU;
            ppu->WriteToRegister(address, word);
        }

        else if(address == OAMDMA_ADDR){

            ppu->WriteToRegister(address, word);
            uint16_t abs_address = word << 8;

            for(uint16_t i = 0x00; i < 0x100; i++){
                ppu->WriteToRegister(OAMDATA_ADDR, this->cpu->ReadFrom(abs_address + i));
            }
        }
    }

    else if(dynamic_cast<PPU*>(processor)){
        if(address >= NAMETABLE_0_START && address <= NAMETABLE_MIRROR_END){

            // Por causa do mirroring das nametables
            if(address >= NAMETABLE_MIRROR_START)
                address -= (4 * NAMETABLES_SIZE);

            if(address <= NAMETABLE_0_END){

                address -= NAMETABLE_0_START;
                this->memory->WritePPURAM(address, word);
            }

            else{
                // Mirroring Horizontal
                if(this->memory->mirroring_type == 0){

                    if(address <= NAMETABLE_2_END)
                        address -= NAMETABLES_SIZE;

                    else
                        address -= (NAMETABLES_SIZE * 2);


                    address -= NAMETABLE_0_START;
                    this->memory->WritePPURAM(address, word);
                }
                // Mirroring Vertical
                else{

                    if(address >= NAMETABLE_2_START && address <= NAMETABLE_3_END)
                        address -= (NAMETABLES_SIZE * 2);

                    address -= NAMETABLE_0_START;
                    this->memory->WritePPURAM(address, word);
                }
            }
        }
        else if(address <= PALLETE_RAM_INDEXES_END){

            address -= PALLETE_RAM_INDEXES_START;
            this->memory->WritePALLETERAM(address, word);
        }
        else{

            address %= PALLETE_RAM_MIRRORING_START;
            this->memory->WritePALLETERAM(address, word);
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
            address = address % INTERNAL_CPU_RAM_SIZE;
			//printf("address = %d", address);
            this->cpu->last_accessed_mem = address;
            value = this->memory->ReadCPURAM(address);
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;

            // Apenas somo pra ficar bonitinho com os valores certos em common/constants.hpp
            address += PPU_REGISTERS_START;

            // Leitura depende de fato da ppu
            value = this->ppu->ReadFromRegister(address);
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

            this->cpu->last_accessed_mem = address;
        }
    }

    else if(dynamic_cast<PPU*>(processor)){

        if(address <= PATTERN_TABLE_1_END){
            if(this->memory->size_CHR_ROM_in_8kb_units){

                value = this->memory->ReadCHRROM(address);
            }
        }
        else if(address >= NAMETABLE_0_START && address <= NAMETABLE_MIRROR_END){

            // Por causa do mirroring das nametables
            if(address >= NAMETABLE_MIRROR_START)
                address -= (4 * NAMETABLES_SIZE);

            if(address <= NAMETABLE_0_END){

                address -= NAMETABLE_0_START;
                value = this->memory->ReadPPURAM(address);
            }

            else{
                // Mirroring Horizontal
                if(this->memory->mirroring_type == 0){

                    if(address <= NAMETABLE_2_END)
                        address -= NAMETABLES_SIZE;

                    else
                        address -= (NAMETABLES_SIZE * 2);


                    address -= NAMETABLE_0_START;
                    value = this->memory->ReadPPURAM(address);
                }
                // Mirroring Vertical
                else{

                    if(address >= NAMETABLE_2_START && address <= NAMETABLE_3_END)
                        address -= (NAMETABLES_SIZE * 2);

                    address -= NAMETABLE_0_START;
                    value = this->memory->ReadPPURAM(address);
                }
            }
        }
        else if(address <= PALLETE_RAM_INDEXES_END){

            address -= PALLETE_RAM_INDEXES_START;
            value = this->memory->ReadPALLETERAM(address);
        }
        else{

            address %= PALLETE_RAM_MIRRORING_START;
            value = this->memory->ReadPALLETERAM(address);
        }
    }

    return value;
}

void AddressBus::GenNMI() {

    this->cpu->time_for_NMI = true;
    this->cpu->ResetNumCycles();
}

