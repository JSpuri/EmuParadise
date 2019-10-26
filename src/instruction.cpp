#include "headers/instruction.hpp"

#include "headers/operations.hpp"
#include "common/constants.hpp"

#include <iostream>

// Instruction class constructor
// It sets the operation function and mode accordingly to opcode
Instruction::Instruction(CPU *cpu, uint8_t opcode) {

    this->cpu = cpu;
    this->opcode = opcode;
    this->access_memory = true;
    switch(opcode){

        //ADC
        case(0x69): //Immediate
            this->mode = M_IMMEDIATE;
            this->operation = ADC;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        case(0x65): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = ADC;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;

        case(0x75): //Zero Page, X
            this->mode = M_ZERO_PAGE_X;
            this->operation = ADC;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;

        case(0x6d): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = ADC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        case(0x7d): //Absolute, X
            this->mode = M_ABSOLUTE_X;
            this->operation = ADC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        case(0x79): //Absolute Y
            this->mode = M_ABSOLUTE_Y;
            this->operation = ADC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        case(0x61): //(Indirect, X)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = ADC;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;

        case(0x71): //(Indirect), Y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = ADC;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        //AND
        case(0x29): //Immediate
            this->mode = M_IMMEDIATE;
            this->operation = AND;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0x25): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = AND;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0x35): //Zero Page, X
            this->mode = M_ZERO_PAGE_X;
            this->operation = AND;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(0x2d): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = AND;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x3d): //Absolute, X
            this->mode = M_ABSOLUTE_X;
            this->operation = AND;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x39): //Absolute, Y
            this->mode = M_ABSOLUTE_Y;
            this->operation = AND;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x21): //(Indirect, X)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = AND;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0x31): //(Indirect), Y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = AND;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        //ASL
        case(0x0A): //Accumulator
            this->mode = M_ACCUMULATOR;
            this->operation = ASL;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0x06): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = ASL;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(0x16): //Zero Page, X
            this->mode = M_ZERO_PAGE_X;
            this->operation = ASL;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0x0e): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = ASL;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(0x1e): //Absolute, X
            this->mode =M_ABSOLUTE_X;
            this->operation = ASL;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        //BCC
        case(0x90):
            this->mode = M_RELATIVE;
            this->operation = BCC;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        //BCS
        case(0xb0):
            this->mode = M_RELATIVE;
            this->operation = BCS;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        //BEQ
        case(0xf0):
            this->mode = M_RELATIVE;
            this->operation = BEQ;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        //BIT
        case(0x24): //Zero Page
            this->mode = M_ZERO_PAGE;
            this->operation = BIT;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0x2c): //Absolute
            this->mode = M_ABSOLUTE;
            this->operation = BIT;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        //BMI
        case(0x30):
            this->mode = M_RELATIVE;
            this->operation = BMI;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        //BNE
        case(0xd0):
            this->mode = M_RELATIVE;
            this->operation = BNE;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        //BPL
        case(0x10):
            this->mode = M_RELATIVE;
            this->operation = BPL;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        //BRK
        case(0x00):
            this->mode = M_IMPLICIT;
            this->operation = BRK;
            this->num_bytes = 1;
            this->num_cycles = 7;
            this->access_memory = false;
            break;

        //BVC
        case(0x50):
            this->mode = M_RELATIVE;
            this->operation = BVC;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        //BVS
        case(0x70):
            this->mode = M_RELATIVE;
            this->operation = BVS;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        //CLC
        case(0x18):
            this->mode = M_IMPLICIT;
            this->operation = CLC;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        ///
        /// CMP
        ///
        case(0xC9): //immediate
            this->mode = M_IMMEDIATE;
            this->operation = CMP;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0xC5): //zero page
            this->mode = M_IMMEDIATE;
            this->operation = CMP;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0xD5): //zero page + x
            this->mode = M_ZERO_PAGE_X;
            this->operation = CMP;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(0xCD): //absolute
            this->mode = M_ABSOLUTE;
            this->operation = CMP;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0xDD): //absolute + x
            this->mode = M_ABSOLUTE_X;
            this->operation = CMP;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0xD9): //absolute + y
            this->mode = M_ABSOLUTE_Y;
            this->operation = CMP;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0xC1): // (indirect, x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = CMP;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0xD1): // (indirect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = CMP;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        ///
        /// CPX
        ///
        case(0xE0): //immediate
            this->mode = M_IMMEDIATE;
            this->operation = CPX;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0xE4): //zero page
            this->mode = M_ZERO_PAGE;
            this->operation = CPX;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0xEC): //absolute
            this->mode = M_ABSOLUTE;
            this->operation = CPX;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        ///
        /// CPY
        ///
        case(0xC0): //immediate
            this->mode = M_IMMEDIATE;
            this->operation = CPY;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0xC4): //zero page
            this->mode = M_ZERO_PAGE;
            this->operation = CPY;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0xCC): //absolute
            this->mode = M_ABSOLUTE;
            this->operation = CPY;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        ///
        /// DEC
        ///
        case(0xC6): // zero page
            this->mode = M_ZERO_PAGE;
            this->operation = DEC;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(0xD6): // zero page + x
            this->mode = M_ZERO_PAGE_X;
            this->operation = DEC;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0xCE): // absolute
            this->mode = M_ABSOLUTE;
            this->operation = DEC;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(0xDE): // absolute + x
            this->mode = M_ABSOLUTE_X;
            this->operation = DEC;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        ///
        /// DEX
        ///
        case(0xCA):
            this->mode = M_IMPLICIT;
            this->operation = DEX;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        ///
        /// DEY
        ///
        case(0x88):
            this->mode = M_IMPLICIT;
            this->operation = DEY;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        ///
        /// EOR
        ///
        case(0x49): //immediate
            this->mode = M_IMMEDIATE;
            this->operation = EOR;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0x45): //zero page
            this->mode = M_ZERO_PAGE;
            this->operation = EOR;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(0x55): //zero page + x
            this->mode = M_ZERO_PAGE_X;
            this->operation = EOR;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(0x4D): //absolute
            this->mode = M_ABSOLUTE;
            this->operation = EOR;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x5D): //absolute + x
            this->mode = M_ABSOLUTE_X;
            this->operation = EOR;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x59): //absolute + y
            this->mode = M_ABSOLUTE_Y;
            this->operation = EOR;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(0x41): // (indirect, x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = EOR;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0x51): // (indirect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = EOR;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        ///
        /// INC
        ///
        case(0xE6): // zero page
            this->mode = M_ZERO_PAGE;
            this->operation = INC;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(0xF6): // zero page + x
            this->mode = M_ZERO_PAGE_X;
            this->operation = INC;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(0xEE): // absolute
            this->mode = M_ABSOLUTE;
            this->operation = INC;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(0xFE): // absolute + x
            this->mode = M_ABSOLUTE_X;
            this->operation = INC;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        ///
        /// INX
        ///
        case(0xE8):
            this->mode = M_IMPLICIT;
            this->operation = INX;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        ///
        /// INY
        ///
        case(0xC8):
            this->mode = M_IMPLICIT;
            this->operation = INY;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        ///
        /// JMP
        ///
        case(0x4C): // absolute
            this->mode = M_ABSOLUTE;
            this->operation = JMP;
            this->num_bytes = 3;
            this->num_cycles = 3;
            this->access_memory = false;
            break;
        case(0x6C): // indirect
            this->mode = M_INDIRECT;
            this->operation = JMP;
            this->num_bytes = 3;
            this->num_cycles = 5;
            this->access_memory = false;
            break;

        //
        //JSR
        //
        case(32): //20
            this->mode = M_ABSOLUTE;
            this->operation = JSR;
            this->num_bytes = 3;
            this->num_cycles = 6;
            this->access_memory = false;
            break;

        //
        //LDA
        //
        case(169): //a9 - immediate
            this->mode = M_IMMEDIATE;
            this->operation = LDA;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(165): //a5 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = LDA;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(181): //b5 - zeropage,x
            this->mode = M_ZERO_PAGE_X;
            this->operation = LDA;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(173): //ad - absolute
            this->mode = M_ABSOLUTE;
            this->operation = LDA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(189): //bd - absolute, x
            this->mode = M_ABSOLUTE_X;
            this->operation = LDA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(185): //b9 - absolute, y
            this->mode = M_ABSOLUTE_Y;
            this->operation = LDA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(161): //a1 - (indirect, x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = LDA;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(177): //b1 - (indrect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = LDA;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        //
        //LDX
        //
        case(162): //a2 - immediate
            this->mode = M_IMMEDIATE;
            this->operation = LDX;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(166): //a6 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = LDX;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(182): //b6 - zeropage,y
            this->mode = M_ZERO_PAGE_Y;
            this->operation = LDX;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(174): //ae - absolute
            this->mode = M_ABSOLUTE;
            this->operation = LDX;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(190): //be - absolute, y
            this->mode = M_ABSOLUTE_Y;
            this->operation = LDX;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        //
        //LDY
        //
        case(160): //a0 - immediate
            this->mode = M_IMMEDIATE;
            this->operation = LDY;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(164): //a4 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = LDY;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(180): //b4 - zeropage, x
            this->mode = M_ZERO_PAGE_X;
            this->operation = LDY;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(172): //ac - absolute
            this->mode = M_ABSOLUTE;
            this->operation = LDY;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(188): //bc - absolute, x
            this->mode = M_ABSOLUTE_X;
            this->operation = LDY;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        //
        //LSR
        //
        case(74): //4a - accumulator
            this->mode = M_ACCUMULATOR;
            this->operation = LSR;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(70): //46 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = LSR;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(86): //56 - zeropage, x
            this->mode = M_ZERO_PAGE_X;
            this->operation = LSR;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(78): //4e - absolute
            this->mode = M_ABSOLUTE;
            this->operation = LSR;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(94): //5e - absolute,x
            this->mode = M_ABSOLUTE_X;
            this->operation = LSR;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        //
        //NOP
        //
        case(234): //ea - implied
            this->mode = M_IMPLICIT;
            this->operation = NOP;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        //
        //ORA
        //
        case(9): //09 - immediate
            this->mode = M_IMMEDIATE;
            this->operation = ORA;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(5): //05 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = ORA;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(21): //15 - zeropage,x
            this->mode = M_ZERO_PAGE_X;
            this->operation = ORA;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(13): //0d - absolute
            this->mode = M_ABSOLUTE;
            this->operation = ORA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(29): //1d - absolute,x
            this->mode = M_ABSOLUTE_X;
            this->operation = ORA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(25): //19 - absolute,y
            this->mode = M_ABSOLUTE_Y;
            this->operation = ORA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(1): //01 - (indirect,x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = ORA;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(17): //11 - (indirect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = ORA;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        //
        //PHA
        //
        case(72): //48 - implied
            this->mode = M_IMPLICIT;
            this->operation = PHA;
            this->num_bytes = 1;
            this->num_cycles = 3;
            break;
        //
        //PHP
        //
        case(8): //08 - implied
            this->mode = M_IMPLICIT;
            this->operation = PHP;
            this->num_bytes = 1;
            this->num_cycles = 3;
            break;
        //
        //PLA
        //
        case(104): //68 - implied
            this->mode = M_IMPLICIT;
            this->operation = PLA;
            this->num_bytes = 1;
            this->num_cycles = 4;
            break;
        //
        //PLP
        //
        case(40): //28 - implied
            this->mode = M_IMPLICIT;
            this->operation = PLP;
            this->num_bytes = 1;
            this->num_cycles = 4;
            break;

        //
        //ROL
        //
        case(42): //2a - accumulator
            this->mode = M_ACCUMULATOR;
            this->operation = ROL;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(38): //26 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = ROL;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(54): //36 - zeropage,x
            this->mode = M_ZERO_PAGE_X;
            this->operation = ROL;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(46): //2e - absolute
            this->mode = M_ABSOLUTE;
            this->operation = ROL;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(62): //3e - absolute,x
            this->mode = M_ABSOLUTE_X;
            this->operation = ROL;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        //
        //ROR
        //
        case(106): //6a - accumulator
            this->mode = M_ACCUMULATOR;
            this->operation = ROR;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(102): //66 - zeropage
            this->mode = M_ZERO_PAGE;
            this->operation = ROR;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;
        case(118): //76 - zeropage,x
            this->mode = M_ZERO_PAGE_X;
            this->operation = ROR;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(110): //6e - absolute
            this->mode = M_ABSOLUTE;
            this->operation = ROR;
            this->num_bytes = 3;
            this->num_cycles = 6;
            break;
        case(126): //7e - absolute,x
            this->mode = M_ABSOLUTE_X;
            this->operation = ROR;
            this->num_bytes = 3;
            this->num_cycles = 7;
            break;

        //
        //RTI
        //
        case(64): //40 - implied
            this->mode = M_IMPLICIT;
            this->operation = RTI;
            this->num_bytes = 1;
            this->num_cycles = 6;
            this->access_memory = false;
            break;

        //RTS
        case(96): 	//60 -- implied
            this->mode = M_IMPLICIT;
            this->operation = RTS;
            this->num_bytes = 1;
            this->num_cycles = 6;
            this->access_memory = false;
            break;

        //SBC
        case(233): 	//e9 -- immediate
            this->mode = M_IMMEDIATE;
            this->operation = SBC;
            this->num_bytes = 2;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(229): 	//e5 -- zero page
            this->mode = M_ZERO_PAGE;
            this->operation = SBC;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(245): 	//f5 -- zero page, x
            this->mode = M_ZERO_PAGE_X;
            this->operation = SBC;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(237): 	//ed -- absolute
            this->mode = M_ABSOLUTE;
            this->operation = SBC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(253): 	//fd -- absolute, x
            this->mode = M_ABSOLUTE_X;
            this->operation = SBC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(249): 	//f9 -- absolute, y
            this->mode = M_ABSOLUTE_Y;
            this->operation = SBC;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(225): 	//e1 -- (indirect, x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = SBC;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(241): 	//f1 -- (indirect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = SBC;
            this->num_bytes = 2;
            this->num_cycles = 5;
            break;

        //SEC
        case(56):		//38 -- implied
            this->mode = M_IMPLICIT;
            this->operation = SEC;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // SED
        case(248):	//f8 -- implied
            this->mode = M_IMPLICIT;
            this->operation = SED;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // SEI
        case(120):	//78 -- implied
            this->mode = M_IMPLICIT;
            this->operation = SEI;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        // STA
        case(133): 	//85 -- zero page
            this->mode = M_ZERO_PAGE;
            this->operation = STA;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(149): 	//95 -- zero page, x
            this->mode = M_ZERO_PAGE_X;
            this->operation = STA;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(141): 	//8d -- absolute
            this->mode = M_ABSOLUTE;
            this->operation = STA;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;
        case(157): 	//9d -- absolute, x
            this->mode = M_ABSOLUTE_X;
            this->operation = STA;
            this->num_bytes = 3;
            this->num_cycles = 5;
            break;
        case(153): 	//99 -- absolute, y
            this->mode = M_ABSOLUTE_Y;
            this->operation = STA;
            this->num_bytes = 3;
            this->num_cycles = 5;
            break;
        case(129): 	//81 -- (indirect, x)
            this->mode = M_INDEXED_INDIRECT;
            this->operation = STA;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;
        case(145): 	//91 -- (indirect), y
            this->mode = M_INDIRECT_INDEXED;
            this->operation = STA;
            this->num_bytes = 2;
            this->num_cycles = 6;
            break;

        // STX
        case(134): 	//86 -- zero page
            this->mode = M_ZERO_PAGE;
            this->operation = STX;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(150): 	//96 -- zero page, y
            this->mode = M_ZERO_PAGE_Y;
            this->operation = STX;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(142): 	//8e -- absolute
            this->mode = M_ABSOLUTE;
            this->operation = STX;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        // STY
        case(132): 	//84 -- zero page
            this->mode = M_ZERO_PAGE;
            this->operation = STY;
            this->num_bytes = 2;
            this->num_cycles = 3;
            break;
        case(148): 	//94 -- zero page, x
            this->mode = M_ZERO_PAGE_X;
            this->operation = STY;
            this->num_bytes = 2;
            this->num_cycles = 4;
            break;
        case(140): 	//8c -- absolute
            this->mode = M_ABSOLUTE;
            this->operation = STY;
            this->num_bytes = 3;
            this->num_cycles = 4;
            break;

        // TAX
        case(170): 	//aa -- implied
            this->mode = M_IMPLICIT;
            this->operation = TAX;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // TAY
        case(168): 	//a8 -- implied
            this->mode = M_IMPLICIT;
            this->operation = TAY;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // TSX
        case(186): 	//ba -- implied
            this->mode = M_IMPLICIT;
            this->operation = TSX;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // TXA
        case(138): 	//8a -- implied
            this->mode = M_IMPLICIT;
            this->operation = TXA;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // TXS
        case(154): 	//9a -- implied
            this->mode = M_IMPLICIT;
            this->operation = TXS;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        // TYA
        case(152): 	//98 -- implied
            this->mode = M_IMPLICIT;
            this->operation = TYA;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        case(0x58): // CLI
            this->mode = M_IMPLICIT;
            this->operation = CLI;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0xB8): // CLV
            this->mode = M_IMPLICIT;
            this->operation = CLV;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;
        case(0xD8): // CLD
            this->mode = M_IMPLICIT;
            this->operation = CLD;
            this->num_bytes = 1;
            this->num_cycles = 2;
            this->access_memory = false;
            break;

        default:
            this->mode = -1;
            this->operation = NULL;
            this->num_bytes = 0;
            this->num_cycles = 0;
            break;
    }

    
}

// Method to execute previously set operation, print log to stdout
// and reset memory->was_accessed to false (Operation must handle
// setting memory->was_acessed to true)
void Instruction::Run() {

    uint16_t last_pc = cpu->pc;

    this->operation(mode, cpu);

    // Add default instructions cycle number to cpu->num_cycles
    cpu->IncrementNumCycles(this->num_cycles);

    if(last_pc == cpu->pc){
        cpu->pc += this->num_bytes;
    }
    else{
        // If instruction is a branch and it succeeded, add one more cpu cycle
        if(this->opcode != 0x4C && this->opcode != 0x6C && this->opcode != 20){
            cpu->IncrementNumCycles();

            // And if ocurred page-crossing, add two more cpu cycles
            if((last_pc & 0xFF00) != (cpu->pc & 0xFF00))
                cpu->IncrementNumCycles(2);
        }
    }

    if (this->access_memory)
        logls();
    else
        log();
}


void Instruction::log(){
	printf("| pc = 0x%04x | a = 0x%02x | x = 0x%02x | y = 0x%02x | sp = 0x01%02x | p[NV-BDIZC] = %d%d%d%d%d%d%d%d |",
			cpu->pc, (uint8_t) cpu->a, (uint8_t) cpu->x, (uint8_t) cpu->y, cpu->sp, cpu->ps[0], cpu->ps[1], cpu->ps[2], cpu->ps[3], cpu->ps[4], cpu->ps[5], cpu->ps[6], cpu->ps[7]);

	printf("\n");
}

void Instruction::logls(){
	printf("| pc = 0x%04x | a = 0x%02x | x = 0x%02x | y = 0x%02x | sp = 0x01%02x | p[NV-BDIZC] = %d%d%d%d%d%d%d%d | MEM[0x%04x] = 0x%02x |",
			cpu->pc, (uint8_t) cpu->a, (uint8_t) cpu->x, (uint8_t) cpu->y, cpu->sp, cpu->ps[0], cpu->ps[1], cpu->ps[2], cpu->ps[3], cpu->ps[4], cpu->ps[5], cpu->ps[6], cpu->ps[7], cpu->last_accessed_mem, cpu->ReadFrom(cpu->last_accessed_mem));

	printf("\n");
}
