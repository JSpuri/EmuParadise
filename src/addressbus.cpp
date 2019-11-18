#include "headers/addressbus.hpp"
#include <iostream>

AddressBus::AddressBus(char *nesfile, CPU *cpu, PPU *ppu) {

    this->system_clock = 0;

    // Verifica o byte que nos diz quantos PRG ROMs de 16kb ele tem
    // Se tem dois, tem 32kb de ROM
    this->size_PRG_ROM_in_16kb_units = nesfile[4];
    printf("Size of PRG ROM in 16kb units: %02x\n", this->size_PRG_ROM_in_16kb_units);

    // Verifica o byte que nos diz quantos CHR ROMs de 8kb ele tem
    // Se tem zero, usa CHR RAM
    this->size_CHR_ROM_in_8kb_units = nesfile[5];
    printf("Size of CHR ROM in 8kb units: %02x\n", this->size_CHR_ROM_in_8kb_units);

    if(this->size_CHR_ROM_in_8kb_units)
        this->size_CHR_RAM_in_8kb_units = 0;
    
    // Determina o tipo de mirroring: 0 eh horizontal, 1 eh vertical
    this->mirroring_type = nesfile[6] & 0X01;
    printf("Mirroring type (0 -> horizontal and 1 -> vertical): %u\n", this->mirroring_type);

    // Verifica o numero mapper do cartucho
    this->mapper_number = (nesfile[7] & 0xF0) + (nesfile[6] >> 4);
    printf("Mapper type: %04x\n", this->mapper_number);

    // Verifica o tamanho da PRG_RAM (em unidades de 8kb)
    this->size_PRG_RAM_in_8kb_units = nesfile[8];
    printf("Size of PRG RAM in 8kb units: %02x\n", this->size_PRG_RAM_in_8kb_units);

    // Redimensiona todos os bancos do cartucho de acordo com o necessario
    uint16_t pgr_rom_size = PRG_ROM_SIZE * this->size_PRG_ROM_in_16kb_units;
    uint16_t chr_rom_size = CHR_ROM_SIZE * this->size_CHR_ROM_in_8kb_units;
    uint16_t pgr_ram_size = PRG_RAM_SIZE * this->size_PRG_RAM_in_8kb_units;
    uint16_t chr_ram_size = CHR_RAM_SIZE * this->size_CHR_RAM_in_8kb_units;
    printf("PRG rom size: %04x\n", pgr_rom_size);
    printf("CHR rom size: %04x\n", chr_rom_size);
    printf("PRG ram size: %04x\n", pgr_ram_size);
    printf("CHR ram size: %04x\n", chr_ram_size);

    // Reserva as memorias e inicializa tudo com zero
    this->INTERNAL_CPU_RAM.resize(INTERNAL_CPU_RAM_SIZE, 0);
    this->INTERNAL_PPU_RAM.resize(INTERNAL_PPU_RAM_SIZE, 0x20);
    this->PRG_ROM.resize(pgr_rom_size, 0);
    this->CHR_ROM.resize(chr_rom_size, 0);
    this->PRG_RAM.resize(pgr_ram_size, 0);
    this->CHR_RAM.resize(chr_ram_size, 0);
    this->PALLETE_RAM.resize(PALLETE_RAM_INDEXES_SIZE, 0);

    // Loop de copia do arquivo binario para o vetor de uint8_t (de fato nossa memoria)
    for(uint16_t i = 0; i < pgr_rom_size; i++){
        this->PRG_ROM[i] = (unsigned char)nesfile[i + 0x10];
        //printf("%02x\n", this->PRG_ROM[i]);
    }


    for(uint16_t i = 0; i < chr_rom_size; i++)
        this->CHR_ROM[i] = (unsigned char)nesfile[i + (0x10 + pgr_rom_size)];

    // Guarda os enderecos de NMI, RESET e IRQ para facilitar no futuro
    this->NMI_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_NMI_ADDR)] << 8;
    this->NMI_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_NMI_ADDR)];
    printf("NMI address: %04x\n", this->NMI_ADDR);

    this->RESET_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_RST_ADDR)] << 8;
    this->RESET_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_RST_ADDR)];
    printf("Reset address: %04x\n", this->RESET_ADDR);

    this->IRQ_ADDR = this->PRG_ROM[(pgr_rom_size - UPR_IRQ_ADDR)] << 8;
    this->IRQ_ADDR += this->PRG_ROM[(pgr_rom_size - LWR_IRQ_ADDR)];
    printf("IRQ address: %04x\n", this->IRQ_ADDR);


    // Inicializa variavel que verifica se a memoria foi acessada
    this->was_accessed = false;
    this->last_accessed_mem = 0;

    this->cpu = cpu;
    this->ppu = ppu;

}

bool AddressBus::Clock() {

    this->ppu->Clock();
    
    if(this->system_clock % 3 == 0)
        this->run_emulation = this->cpu->Clock();

    if(this->ppu->nmi){
        this->ppu->nmi = false;
        this->cpu->NMI();
    }

    this->system_clock++;

    return this->run_emulation;
}

// Analisa qual classe quer escrever e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
void AddressBus::WriteTo(int processorType, uint16_t address, uint8_t word) {

    if(processorType == 0){
        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address%PRG_RAM_SIZE;

            this->cpu->last_accessed_mem = address;
            this->INTERNAL_CPU_RAM[address] = word;
        }
        else if(address < PPU_REGISTERS_MIRROR_END){
            //the % operator is due to the mirroring of the registers on
            //$2008-$3FFF
            address -= PPU_REGISTERS_START;
            address = address%PPU_REGISTERS_NUMBER;

            // Apenas somo pra ficar bonitinho com os valores certos em common/constants.hpp
            address += PPU_REGISTERS_START;

            // Escrita de fato depende da PPU;
            //if(word)
                //printf("Write to register %02x: %02x\n", address, word);
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

    else if(processorType == 1){
        if(address >= NAMETABLE_0_START && address <= NAMETABLE_MIRROR_END){

            // Por causa do mirroring das nametables
            if(address >= NAMETABLE_MIRROR_START)
                address -= (4 * NAMETABLES_SIZE);

            if(address <= NAMETABLE_0_END){

                address -= NAMETABLE_0_START;
                this->INTERNAL_PPU_RAM[address] = word;
            }

            else{
                // Mirroring Horizontal
                if(this->mirroring_type == 0){

                    if(address <= NAMETABLE_2_END)
                        address -= NAMETABLES_SIZE;

                    else
                        address -= (NAMETABLES_SIZE * 2);


                    address -= NAMETABLE_0_START;
                    this->INTERNAL_PPU_RAM[address] = word;
                }
                // Mirroring Vertical
                else{

                    if(address >= NAMETABLE_2_START && address <= NAMETABLE_3_END)
                        address -= (NAMETABLES_SIZE * 2);

                    address -= NAMETABLE_0_START;
                    this->INTERNAL_PPU_RAM[address] = word;
                }
            }
        }
        else if(address <= PALLETE_RAM_INDEXES_END){

            address -= PALLETE_RAM_INDEXES_START;
            this->PALLETE_RAM[address] = word;
        }
        else{

            address %= PALLETE_RAM_MIRRORING_START;
        }
    }
}

// Analisa de qual classe ler e, a partir dai, redireciona o valor
// para o local correto, de acordo com o endereco dado.
uint8_t AddressBus::ReadFrom(int processorType, uint16_t address) {

    uint8_t value = 0;


    if(processorType == 0){
        if(address < INTERNAL_CPU_RAM_ENDING){
            //the % operator is due to the mirroring of the ram on
            //$0800-$0FFF, $1000-$17FF and $1800-1FFF
            address = address % INTERNAL_CPU_RAM_SIZE;
            this->cpu->last_accessed_mem = address;
            value = this->INTERNAL_CPU_RAM[address];
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

            this->cpu->last_accessed_mem = address;
        }
    }

    else if(processorType == 1){

        if(address <= PATTERN_TABLE_1_END){
            if(this->size_CHR_ROM_in_8kb_units){

                value = this->CHR_ROM[address];
            }
        }
        else if(address >= NAMETABLE_0_START && address <= NAMETABLE_MIRROR_END){

            // Por causa do mirroring das nametables
            if(address >= NAMETABLE_MIRROR_START)
                address -= (4 * NAMETABLES_SIZE);

            if(address <= NAMETABLE_0_END){

                address -= NAMETABLE_0_START;
                value = this->INTERNAL_PPU_RAM[address];
            }

            else{
                // Mirroring Horizontal
                if(this->mirroring_type == 0){

                    if(address >= NAMETABLE_1_START && address <= NAMETABLE_1_END)
                        address -= NAMETABLES_SIZE;

                    else if(address >= NAMETABLE_2_START && address <= NAMETABLE_2_END)
                        address -= NAMETABLES_SIZE;

                    else if(address >= NAMETABLE_3_START && address <= NAMETABLE_3_END)
                        address -= (NAMETABLES_SIZE * 2);


                    address -= NAMETABLE_0_START;
                    value = this->INTERNAL_PPU_RAM[address];
                }
                // Mirroring Vertical
                else{

                    if(address >= NAMETABLE_2_START && address <= NAMETABLE_3_END)
                        address -= (NAMETABLES_SIZE * 2);

                    address -= NAMETABLE_0_START;
                    value = this->INTERNAL_PPU_RAM[address];
                }
            }
        }
        else if(address <= PALLETE_RAM_INDEXES_END){

            address -= PALLETE_RAM_INDEXES_START;
            //printf("QUERO UMA COR [%04x]\n", address);
            value = this->PALLETE_RAM[address];
            //printf("PEGUEI A COR: %02x\n", value);
        }
        else{
            //printf("QUERO UMA COR 2\n");

            address %= PALLETE_RAM_MIRRORING_START;
        }
    }

    return value;
}

void AddressBus::GenNMI() {

    this->cpu->time_for_NMI = true;
}

