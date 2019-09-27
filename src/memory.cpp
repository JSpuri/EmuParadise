#include <bits/stdc++.h>

#define PRG_RAM_SIZE 0x0800
#define PRG_ROM_SIZE 0x4000

#define PRG_ROM_1_BANK_START 0x8000
#define PRG_ROM_2_BANK_START 0xC000

#define LWR_NMI_ADDR 6
#define UPR_NMI_ADDR 5

#define LWR_RST_ADDR 4
#define UPR_RST_ADDR 3

#define LWR_IRQ_ADDR 2
#define UPR_IRQ_ADDR 1

class Memory {

    public:
        Memory(char *nesfile);       // Constructor
        int8_t read(uint16_t addr);
        void write(uint16_t addr, int8_t value);

        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

    private:
        std::vector<uint8_t> PRG_RAM;
        std::vector<uint8_t> PRG_ROM;
        bool has_32kb_PRG_ROM;

};

Memory::Memory(char *nesfile) {

    bool has32kbPRG_ROM = ((nesfile[4] == 2) ? true : false);
    
    //if the cartridge has 32KB, we must double the PRG_ROM size
    uint16_t rom_size =  PRG_ROM_SIZE * ((has32kbPRG_ROM) ? 2 : 1);

    this->PRG_RAM.resize(PRG_RAM_SIZE, 0);
    this->PRG_ROM.resize(rom_size, 0);

    this->has_32kb_PRG_ROM = has32kbPRG_ROM;
    
    for(uint16_t i = 0; i < rom_size; i++)
        this->PRG_ROM[i] = nesfile[i + 0x10];

    this->NMI_ADDR = this->PRG_ROM[(rom_size - UPR_NMI_ADDR)] << 8;
    this->NMI_ADDR += this->PRG_ROM[(rom_size - LWR_NMI_ADDR)];

    this->RESET_ADDR = this->PRG_ROM[(rom_size - UPR_RST_ADDR)] << 8;
    this->RESET_ADDR += this->PRG_ROM[(rom_size - LWR_RST_ADDR)];

    this->IRQ_ADDR = this->PRG_ROM[(rom_size - UPR_IRQ_ADDR)] << 8;
    this->IRQ_ADDR += this->PRG_ROM[(rom_size - LWR_IRQ_ADDR)];

    /*
    printf("%02x %02x %02x\n", this->NMI_ADDR, this->RESET_ADDR, this->IRQ_ADDR);
    for(uint16_t i = 0; i < rom_size; i++)
        printf("%02x ", this->PRG_ROM[i]);
    */

}

int8_t Memory::read(uint16_t addr) {

    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if (addr < 0x2000)
        return this->PRG_RAM[addr%PRG_RAM_SIZE];

    if (this->has_32kb_PRG_ROM)
        return this->PRG_ROM[addr - PRG_ROM_1_BANK_START];

    //if the cartridge doesn't have 32KB, we must mirror
    //$8000-$BFFF and $C000-$FFFF
    else{

        if (addr >= PRG_ROM_1_BANK_START && addr < PRG_ROM_2_BANK_START)
            return this->PRG_ROM[addr - PRG_ROM_1_BANK_START];

        else if (addr >= PRG_ROM_2_BANK_START && addr <= 0xFFFF)
            return this->PRG_ROM[addr - PRG_ROM_1_BANK_START - PRG_ROM_SIZE];
    }

    return 0;

}

void Memory::write(uint16_t addr, int8_t value) {

    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if(addr < 0x2000)
        this->PRG_RAM[addr%PRG_RAM_SIZE] = value;
    
}

