#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <cstdlib>

#include "operations.hpp"
#include "cpu.hpp"

void setInstructionProperties(uint8_t opcode, CPU *cpu){
    switch(opcode) {
        //ADC
        case(0x69): //Immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        case(0x65): //Zero Page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;

        case(0x75): //Zero Page, X
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;

        case(0x6d): //Absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        case(0x7d): //Absolute, X
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        case(0x79): //Absolute Y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        case(0x61): //(Indirect, X)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;

        case(0x71): //(Indirect), Y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(ADC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        //AND
        case(0x29): //Immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0x25): //Zero Page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0x35): //Zero Page, X
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x2d): //Absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x3d): //Absolute, X
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x39): //Absolute, Y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x21): //(Indirect, X)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0x31): //(Indirect), Y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(AND);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        //ASL
        case(0x0A): //Accumulator
            cpu->setInstructionMode(M_ACCUMULATOR);
            cpu->setOperation(ASL);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0x06): //Zero Page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(ASL);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(0x16): //Zero Page, X
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(ASL);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0x0e): //Absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(ASL);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(0x1e): //Absolute, X
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(ASL);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        //BCC
        case(0x90):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BCC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        //BCS
        case(0xb0):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BCS);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        //BEQ
        case(0xf0):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BEQ);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        //BIT
        case(0x24): //Zero Page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(BIT);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0x2c): //Absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(BIT);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        //BMI
        case(0x30):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BMI);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        //BNE
        case(0xd0):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BNE);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        //BPL
        case(0x10):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BPL);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        //BRK
        case(0x00):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(BRK);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(7);
            cpu->setInstructionAccessedMemory(false);
            break;

        //BVC
        case(0x50):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BVC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        //BVS
        case(0x70):
            cpu->setInstructionMode(M_RELATIVE);
            cpu->setOperation(BVS);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        //CLC
        case(0x18):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(CLC);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        ///
        /// CMP
        ///
        case(0xC9): //immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0xC5): //zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0xD5): //zero page + x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(0xCD): //absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0xDD): //absolute + x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0xD9): //absolute + y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0xC1): // (indirect, x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0xD1): // (indirect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(CMP);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        ///
        /// CPX
        ///
        case(0xE0): //immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(CPX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0xE4): //zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(CPX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0xEC): //absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(CPX);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        ///
        /// CPY
        ///
        case(0xC0): //immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(CPY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0xC4): //zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(CPY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0xCC): //absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(CPY);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        ///
        /// DEC
        ///
        case(0xC6): // zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(DEC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(0xD6): // zero page + x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(DEC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0xCE): // absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(DEC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(0xDE): // absolute + x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(DEC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        ///
        /// DEX
        ///
        case(0xCA):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(DEX);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        ///
        /// DEY
        ///
        case(0x88):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(DEY);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        ///
        /// EOR
        ///
        case(0x49): //immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0x45): //zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(0x55): //zero page + x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x4D): //absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x5D): //absolute + x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x59): //absolute + y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(0x41): // (indirect, x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0x51): // (indirect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(EOR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        ///
        /// INC
        ///
        case(0xE6): // zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(INC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(0xF6): // zero page + x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(INC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(0xEE): // absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(INC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(0xFE): // absolute + x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(INC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        ///
        /// INX
        ///
        case(0xE8):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(INX);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        ///
        /// INY
        ///
        case(0xC8):
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(INY);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        ///
        /// JMP
        ///
        case(0x4C): // absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(JMP);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(3);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0x6C): // indirect
            cpu->setInstructionMode(M_INDIRECT);
            cpu->setOperation(JMP);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(5);
            cpu->setInstructionAccessedMemory(false);
            break;

        //
        //JSR
        //
        case(32): //20
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(JSR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            cpu->setInstructionAccessedMemory(false);
            break;

        //
        //LDA
        //
        case(169): //a9 - immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(165): //a5 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(181): //b5 - zeropage,x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(173): //ad - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(189): //bd - absolute, x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(185): //b9 - absolute, y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(161): //a1 - (indirect, x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(177): //b1 - (indrect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(LDA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        //
        //LDX
        //
        case(162): //a2 - immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(LDX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(166): //a6 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(LDX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(182): //b6 - zeropage,y
            cpu->setInstructionMode(M_ZERO_PAGE_Y);
            cpu->setOperation(LDX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(174): //ae - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(LDX);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(190): //be - absolute, y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(LDX);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        //
        //LDY
        //
        case(160): //a0 - immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(LDY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(164): //a4 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(LDY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(180): //b4 - zeropage, x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(LDY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(172): //ac - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(LDY);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(188): //bc - absolute, x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(LDY);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        //
        //LSR
        //
        case(74): //4a - accumulator
            cpu->setInstructionMode(M_ACCUMULATOR);
            cpu->setOperation(LSR);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(70): //46 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(LSR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(86): //56 - zeropage, x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(LSR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(78): //4e - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(LSR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(94): //5e - absolute,x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(LSR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        //
        //NOP
        //
        case(234): //ea - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(NOP);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        //
        //ORA
        //
        case(9): //09 - immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(5): //05 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(21): //15 - zeropage,x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(13): //0d - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(29): //1d - absolute,x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(25): //19 - absolute,y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(1): //01 - (indirect,x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(17): //11 - (indirect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(ORA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        //
        //PHA
        //
        case(72): //48 - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(PHA);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(3);
            break;
        //
        //PHP
        //
        case(8): //08 - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(PHP);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(3);
            break;
        //
        //PLA
        //
        case(104): //68 - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(PLA);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(4);
            break;
        //
        //PLP
        //
        case(40): //28 - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(PLP);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(4);
            break;

        //
        //ROL
        //
        case(42): //2a - accumulator
            cpu->setInstructionMode(M_ACCUMULATOR);
            cpu->setOperation(ROL);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(38): //26 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(ROL);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(54): //36 - zeropage,x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(ROL);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(46): //2e - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(ROL);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(62): //3e - absolute,x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(ROL);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        //
        //ROR
        //
        case(106): //6a - accumulator
            cpu->setInstructionMode(M_ACCUMULATOR);
            cpu->setOperation(ROR);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(102): //66 - zeropage
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(ROR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;
        case(118): //76 - zeropage,x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(ROR);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(110): //6e - absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(ROR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(6);
            break;
        case(126): //7e - absolute,x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(ROR);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(7);
            break;

        //
        //RTI
        //
        case(64): //40 - implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(RTI);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(6);
            cpu->setInstructionAccessedMemory(false);
            break;

        //RTS
        case(96): 	//60 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(RTS);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(6);
            cpu->setInstructionAccessedMemory(false);
            break;

        //SBC
        case(233): 	//e9 -- immediate
            cpu->setInstructionMode(M_IMMEDIATE);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(229): 	//e5 -- zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(245): 	//f5 -- zero page, x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(237): 	//ed -- absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(253): 	//fd -- absolute, x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(249): 	//f9 -- absolute, y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(225): 	//e1 -- (indirect, x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(241): 	//f1 -- (indirect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(SBC);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(5);
            break;

        //SEC
        case(56):		//38 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(SEC);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // SED
        case(248):	//f8 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(SED);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // SEI
        case(120):	//78 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(SEI);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        // STA
        case(133): 	//85 -- zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(149): 	//95 -- zero page, x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(141): 	//8d -- absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;
        case(157): 	//9d -- absolute, x
            cpu->setInstructionMode(M_ABSOLUTE_X);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(5);
            break;
        case(153): 	//99 -- absolute, y
            cpu->setInstructionMode(M_ABSOLUTE_Y);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(5);
            break;
        case(129): 	//81 -- (indirect, x)
            cpu->setInstructionMode(M_INDEXED_INDIRECT);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;
        case(145): 	//91 -- (indirect), y
            cpu->setInstructionMode(M_INDIRECT_INDEXED);
            cpu->setOperation(STA);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(6);
            break;

        // STX
        case(134): 	//86 -- zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(STX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(150): 	//96 -- zero page, y
            cpu->setInstructionMode(M_ZERO_PAGE_Y);
            cpu->setOperation(STX);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(142): 	//8e -- absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(STX);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        // STY
        case(132): 	//84 -- zero page
            cpu->setInstructionMode(M_ZERO_PAGE);
            cpu->setOperation(STY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(3);
            break;
        case(148): 	//94 -- zero page, x
            cpu->setInstructionMode(M_ZERO_PAGE_X);
            cpu->setOperation(STY);
            cpu->setInstructionNumBytes(2);
            cpu->setInstructionNumCycles(4);
            break;
        case(140): 	//8c -- absolute
            cpu->setInstructionMode(M_ABSOLUTE);
            cpu->setOperation(STY);
            cpu->setInstructionNumBytes(3);
            cpu->setInstructionNumCycles(4);
            break;

        // TAX
        case(170): 	//aa -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TAX);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // TAY
        case(168): 	//a8 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TAY);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // TSX
        case(186): 	//ba -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TSX);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // TXA
        case(138): 	//8a -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TXA);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // TXS
        case(154): 	//9a -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TXS);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        // TYA
        case(152): 	//98 -- implied
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(TYA);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        case(0x58): // CLI
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(CLI);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0xB8): // CLV
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(CLV);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;
        case(0xD8): // CLD
            cpu->setInstructionMode(M_IMPLICIT);
            cpu->setOperation(CLD);
            cpu->setInstructionNumBytes(1);
            cpu->setInstructionNumCycles(2);
            cpu->setInstructionAccessedMemory(false);
            break;

        default:
            printf("chegou aqui %02x\n", opcode);
            cpu->setInstructionMode(-1);
            cpu->setOperation(NULL);
            cpu->setInstructionNumBytes(0);
            cpu->setInstructionNumCycles(0);
            break;
    }
}

#endif
