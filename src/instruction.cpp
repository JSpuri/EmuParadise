#include "headers/instruction.hpp"

#include <bits/stdc++.h>

#include "headers/operations.hpp"
#include "common/constants.hpp"

Instruction::Instruction(CPU *cpu, Memory *memory, uint8_t opcode) {

	this->memory = memory;
    this->cpu = cpu;

    switch(opcode){

        //ADC
        case(0x69): //Immediate
            this->mode = M_IMMEDIATE;
            this->operation = ADC;
            break;

        case(0x65): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = ADC;
            break;

        case(0x75): //Zero Page, X
            this->mode = M_ZERO_PAGE_X;
            this->operation = ADC;
            break;

        case(0x6d): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = ADC;
            break;

        case(0x7d): //Absolute, X
            this->mode = M_ABSOLUTE_X;
            this->operation = ADC;
            break;

        case(0x79): //Absolute Y
            this->mode = M_ABSOLUTE_Y;
            this->operation = ADC;
            break;

        case(0x61): //(Indirect, X)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = ADC;
            break;

        case(0x71): //(Indirect), Y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = ADC;
            break;

        //AND
        case(0x29): //Immediate
            this->mode = M_IMMEDIATE;
            this->operation = AND;
            break;
        case(0x25): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = AND;
            break;
        case(0x35): //Zero Page, X
            this->mode = M_ZERO_PAGE_X;
            this->operation = AND;
            break;
        case(0x2d): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = AND;
            break;
        case(0x3d): //Absolute, X
            this->mode = M_ABSOLUTE_X;
            this->operation = AND;
            break;
        case(0x39): //Absolute, Y
            this->mode = M_ABSOLUTE_Y;
            this->operation = AND;
            break;
        case(0x21): //(Indirect, X)
            this->mode = M_IMMEDIATE;
            this->operation = AND;
            break;
        case(0x31): //(Indirect), Y
            this->mode = M_IMMEDIATE;
            this->operation = AND;
            break;

        //ASL
        case(0x0A): //Accumulator
        case(0x06): //Zero Page
        case(0x16): //Zero Page, X
        case(0x0e): //Absolute
        case(0x1e): //Absolute, X

        //BCC
        case(0x90):
        //BCS
        case(0xb0):
        //BEQ
        case(0xf0):
        //BIT
        case(0x24): //Zero Page
        case(0x2c): //Absolute
        //BMI
        case(0x30):
        //BNE
        case(0xd0):
        //BPL
        case(0x10):
        //BRK
        case(0x00):
        //BVC
        case(0x50):
        //BVS
        case(0x70):

        //CLC
        case(0x18):

        ///
        /// CMP
        ///
        case(0xC9): //immediate
        case(0xC5): //zero page
        case(0xD5): //zero page + x
        case(0xCD): //absolute
        case(0xDD): //absolute + x
        case(0xD9): //absolute + y
        case(0xC1): // (indirect, x)
        case(0xD1): // (indirect), y

        ///
        /// CPX
        ///
        case(0xE0): //immediate
        case(0xE4): //zero page
        case(0xEC): //absolute

        ///
        /// CPY
        ///
        case(0xC0): //immediate
        case(0xC4): //zero page
        case(0xCC): //absolute

        ///
        /// DEC
        ///
        case(0xC6): // zero page
        case(0xD6): // zero page + x
        case(0xCE): // absolute
        case(0xDE): // absolute + x

        ///
        /// DEX
        ///
        case(0xCA):
        ///
        /// DEY
        ///
        case(0x88):

        ///
        /// EOR
        ///
        case(0x49): //immediate
        case(0x45): //zero page
        case(0x55): //zero page + x
        case(0x4D): //absolute
        case(0x5D): //absolute + x
        case(0x59): //absolute + y
        case(0x41): // (indirect, x)
        case(0x51): // (indirect), y

        ///
        /// INC
        ///
        case(0xE6): // zero page
        case(0xF6): // zero page + x
        case(0xEE): // absolute
        case(0xFE): // absolute + x

        ///
        /// INX
        ///
        case(0xE8):
        ///
        /// INY
        ///
        case(0xC8):

        ///
        /// JMP
        ///
        case(0x4C): // absolute
        case(0x6C): // indirect

        //
        //JSR
        //
        case(32): //20

        //
        //LDA
        //
        case(169): //a9 - immediate
        case(165): //a5 - zeropage
        case(181): //b5 - zeropage,x
        case(173): //ad - absolute
        case(189): //bd - absolute, x
        case(185): //b9 - absolute, y
        case(161): //a1 - (indirect, x)
        case(177): //b1 - (indrect), y

        //
        //LDX
        //
        case(162): //a2 - immediate
        case(166): //a6 - zeropage
        case(182): //b6 - zeropage,y
        case(174): //ae - absolute
        case(190): //be - absolute, y

        //
        //LDY
        //
        case(160): //a0 - immediate
        case(164): //a4 - zeropage
        case(180): //b4 - zeropage, x
        case(172): //ac - absolute
        case(188): //bc - absolute, x

        //
        //LSR
        //
        case(74): //4a - accumulator
        case(70): //46 - zeropage
        case(86): //56 - zeropage, x
        case(78): //4e - absolute
        case(94): //5e - absolute,x

        //
        //NOP
        //
        case(234): //ea - implied

        //
        //ORA
        //
        case(9): //09 - immediate
        case(5): //05 - zeropage
        case(21): //15 - zeropage,x
        case(13): //0d - absolute
        case(29): //1d - absolute,x
        case(25): //19 - absolute,y
        case(1): //01 - (indirect,x)
        case(17): //11 - (indirect), y

        //
        //PHA
        //
        case(72): //48 - implied
        //
        //PHP
        //
        case(8): //08 - implied
        //
        //PLA
        //
        case(104): //68 - implied
        //
        //PLP
        //
        case(40): //28 - implied

        //
        //ROL
        //
        case(42): //2a - accumulator
        case(38): //26 - zeropage
        case(54): //36 - zeropage,x
        case(46): //2e - absolute
        case(62): //3e - absolute,x

        //
        //ROR
        //
        case(106): //6a - accumulator
        case(102): //66 - zeropage
        case(118): //76 - zeropage,x
        case(110): //6e - absolute
        case(126): //7e - absolute,x

        //
        //RTI
        //
        case(64): //40 - implied

        //RTS
        case(96): 	//60 -- implied

        //SBC
        case(233): 	//e9 -- immediate
        case(229): 	//e5 -- zero page
        case(245): 	//f5 -- zero page, x
        case(237): 	//ed -- absolute
        case(253): 	//fd -- absolute, x
        case(249): 	//f9 -- absolute, y
        case(225): 	//e1 -- (indirect, x)
        case(241): 	//f1 -- (indirect), y

        //SEC
        case(56):		//38 -- implied
        // SED
        case(248):	//f8 -- implied
        // SEI
        case(120):	//78 -- implied

        // STA
        case(133): 	//85 -- zero page
        case(149): 	//95 -- zero page, x
        case(141): 	//8d -- absolute
        case(157): 	//9d -- absolute, x
        case(153): 	//99 -- absolute, y
        case(129): 	//81 -- (indirect, x)
        case(145): 	//91 -- (indirect), y

        // STX
        case(134): 	//86 -- zero page
        case(150): 	//96 -- zero page, y
        case(142): 	//8e -- absolute

        // STY
        case(132): 	//84 -- zero page
        case(148): 	//94 -- zero page, x
        case(140): 	//8c -- absolute

        // TAX
        case(170): 	//aa -- implied
        // TAY
        case(168): 	//a8 -- implied
        // TSX
        case(186): 	//ba -- implied
        // TXA
        case(138): 	//8a -- implied
        // TXS
        case(154): 	//9a -- implied
        // TYA
        case(152): 	//98 -- implied

        case(0x58): // CLI
        case(0xB8): // CLV
        case(0xD8): // CLD

        default:
            break;
    }
    
}

void Instruction::Run() {

    this->operation(mode, cpu, memory);

    if (memory->was_accessed)
        logls();
    else
        log();

    memory->was_accessed = false;
}


void Instruction::log(){
	printf("| pc = 0x%04x | a = 0x%02x | x = 0x%02x | y = 0x%02x | sp = 0x01%02x | p[NV-BDIZC] = %d%d%d%d%d%d%d%d |",
			cpu->pc, (uint8_t) cpu->a, (uint8_t) cpu->x, (uint8_t) cpu->y, cpu->sp, cpu->ps[0], cpu->ps[1], cpu->ps[2], cpu->ps[3], cpu->ps[4], cpu->ps[5], cpu->ps[6], cpu->ps[7]);

	printf("\n");
}

void Instruction::logls(){
	printf("| pc = 0x%04x | a = 0x%02x | x = 0x%02x | y = 0x%02x | sp = 0x01%02x | p[NV-BDIZC] = %d%d%d%d%d%d%d%d | MEM[0x%04x] = 0x%02x |",
			cpu->pc, (uint8_t) cpu->a, (uint8_t) cpu->x, (uint8_t) cpu->y, cpu->sp, cpu->ps[0], cpu->ps[1], cpu->ps[2], cpu->ps[3], cpu->ps[4], cpu->ps[5], cpu->ps[6], cpu->ps[7], memory->last_accessed_mem, memory->read(memory->last_accessed_mem));

	printf("\n");
}
