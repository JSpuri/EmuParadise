#ifndef MEM_H
#define MEM_H

#include <bits/stdc++.h>

// Tamanho dos blocos de RAM e ROM
// Servem para alocar os vetores e fazer
// calculos de mirroring
#define PRG_RAM_SIZE 0x0800
#define PRG_ROM_SIZE 0x4000

// Endereco de inicio de cada um dos bancos de 16kb
// Servem para os calculos de mirroring tambem
#define PRG_ROM_1_BANK_START 0x8000
#define PRG_ROM_2_BANK_START 0xC000

// Esses sao os vetores de cada uma das labels
// NIM, RESET e IRQ, de tras para frente 
// (eles sempre ficam no final da memoria (0xfffa)
#define LWR_NMI_ADDR 6
#define UPR_NMI_ADDR 5

#define LWR_RST_ADDR 4
#define UPR_RST_ADDR 3

#define LWR_IRQ_ADDR 2
#define UPR_IRQ_ADDR 1

// Classe memoria
class Memory {

    public:
        // Methods
        Memory(char *nesfile);       // Constructor
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, int8_t value);

        // Attributes
        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_accessed_mem;
        bool was_accessed;

    private:
        std::vector<uint8_t> PRG_RAM;
        std::vector<uint8_t> PRG_ROM;
        bool has_32kb_PRG_ROM;

};


#endif
