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
        bool wasWritten();          // returns was_written

        // Attributes
        uint16_t NMI_ADDR;
        uint16_t RESET_ADDR;
        uint16_t IRQ_ADDR;

        uint16_t last_written_mem;

    private:
        std::vector<uint8_t> PRG_RAM;
        std::vector<uint8_t> PRG_ROM;
        bool has_32kb_PRG_ROM;
        bool was_written;       //store if any memory was altered

};

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


    // Inicializa variavel que verifica se a memoria foi escrita
    this->was_written = false;
    this->last_written_mem = 0;

}

// Le um determinado endereço da memoria. Nao eh necessario um cast ao entrar um zero_pg_addr
uint8_t Memory::read(uint16_t addr) {

    //reset boolean
    this->was_written = false;

    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if (addr < 0x2000)
        return this->PRG_RAM[addr];

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

// Escreve um valor de 8 bits num endereço. Nao eh necessario um cast ao entrar um zero_pg_addr
void Memory::write(uint16_t addr, int8_t value) {

    //set boolean and las written address
    this->was_written = true;
    this->last_written_mem = addr;

    //the % operator is due to the mirroring of the ram on
    //$0800-$0FFF, $1000-$17FF and $1800-1FFF
    if(addr < 0x2000){
        this->PRG_RAM[addr%PRG_RAM_SIZE] = value;
    }
    
}

// Retorna se a memoria sofreu uma escrita ou nao.
bool Memory::wasWritten(){
    return this->was_written;
}
