#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// ========================================================== Instruction Modes
#define M_IMPLICIT 1
#define M_ACCUMULATOR 2
#define M_IMMEDIATE 3
#define M_ZERO_PAGE 4
#define M_ZERO_PAGE_X 5
#define M_ZERO_PAGE_Y 6
#define M_RELATIVE 7
#define M_ABSOLUTE 8
#define M_ABSOLUTE_X 9
#define M_ABSOLUTE_Y 10
#define M_INDIRECT 11
#define M_INDEXED_INDIRECT 12
#define M_INDIRECT_INDEXED 13

// ========================================================== Memory Info
#define PRG_ROM_SIZE 0x4000
#define CHR_ROM_SIZE 0x2000
#define PRG_RAM_SIZE 0x2000
#define CHR_RAM_SIZE 0x2000

// ========================================================== CPU Memory Map
// Tamanho dos blocos de RAM e ROM
// Servem para alocar os vetores e fazer
// calculos de mirroring
#define INTERNAL_CPU_RAM_SIZE 0x0800

#define INTERNAL_CPU_RAM_ENDING 0x2000
// Endereco de inicio de cada um dos bancos de 16kb
// Servem para os calculos de mirroring tambem
#define PRG_ROM_1_BANK_START 0x8000
#define PRG_ROM_2_BANK_START 0xC000

// Enderecos relativos aos registradores da PPU a partir da CPU
#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTERS_MIRROR_END 0x4000
#define PPU_REGISTERS_NUMBER 0x2000

#define PPUCTRL_ADDR 0x2000
#define PPUMASK_ADDR 0x2001
#define PPUSTATUS_ADDR 0x2002
#define OAMADDR_ADDR 0x2003
#define OAMDATA_ADDR 0x2004
#define PPUSCROLL_ADDR 0x2005
#define PPUADDR_ADDR 0x2006
#define PPUDATA_ADDR 0x2007
#define OAMDMA_ADDR 0x4014

// ========================================================== CPU Misc Info
// Esses sao os vetores de cada uma das labels
// NIM, RESET e IRQ, de tras para frente 
// (eles sempre ficam no final da memoria (0xfffa)
#define LWR_NMI_ADDR 6
#define UPR_NMI_ADDR 5

#define LWR_RST_ADDR 4
#define UPR_RST_ADDR 3

#define LWR_IRQ_ADDR 2
#define UPR_IRQ_ADDR 1

// ========================================================== PPU Misc Info
// Temos apenas duas nametables - mirroring ocorre
#define INTERNAL_PPU_RAM_SIZE 0x0800
#define PPUADDR_WRITE_ADDR_LIMIT 0x4000
#define SIZE_PPU_OAM 64

#define SCREEN_SIZE_X 256
#define SCREEN_SIZE_Y 240
// ========================================================== OAM Info
// OAM stores sprites information - 64 sprites * 4 fields of info = 0x100 spaces, [0, 0xFF]
#define OAM_SIZE 0x100

// ========================================================== CPU Memory Map
#define PATTERN_TABLE_0_START 0x0000 
#define PATTERN_TABLE_0_END 0x0FFF

#define PATTERN_TABLE_1_START 0x1000 
#define PATTERN_TABLE_1_END 0x1FFF
#define PATTERN_TABLES_SIZE 0x1000

#define NAMETABLE_0_START 0X2000
#define NAMETABLE_0_END 0x23FF

#define NAMETABLE_1_START 0X2400
#define NAMETABLE_1_END 0x27FF

#define NAMETABLE_2_START 0X2800
#define NAMETABLE_2_END 0x2BFF

#define NAMETABLE_3_START 0X2C00
#define NAMETABLE_3_END 0x2FFF
#define NAMETABLES_SIZE 0x0400

#define NAMETABLE_MIRROR_START 0x3000
#define NAMETABLE_MIRROR_END 0x3EFF
#define NAMETABLE_MIRROR_SIZE 0x0F00

#define PALLETE_RAM_INDEXES_START 0x3F00
#define PALLETE_RAM_INDEXES_END 0x3F1F
#define PALLETE_RAM_INDEXES_SIZE 0x0020

#define PALLETE_RAM_MIRRORING_START 0x3F20
#define PALLETE_RAM_MIRRORING_END 0x3FFF
#define PALLETE_RAM_MIRRORING_SIZE 0x00E0
#endif
