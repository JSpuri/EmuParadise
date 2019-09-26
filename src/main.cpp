#include <stdlib.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
#include <stack>

#include "structures.h"

using namespace std;


struct Instrucao {
	int opcode;
};

Instrucao Decodifica(int pc, char * memblock){
	int opcode = 0;
	
	for(int i = 0; i < 8; i++) {
		opcode = opcode + pow(2, 7-i)*(!!((memblock[pc] << i) & 0x80));
	}
	Instrucao instrucao;
	instrucao.opcode = opcode;
	return instrucao;
}

char readNextByte(int pc, char *memblock) {
	char byte = 0;

	for(int i = 0; i < 8; i++) {
		byte = byte + pow(2, 7-i)*(!!((memblock[pc] << i) & 0x80));
	}
	return byte;
}

void log(CPU cpu){
	printf("| pc = 0x%04x | a = 0x%02x | x = 0x%02x | y = 0x%02x | sp = 0x%04x | p[NV-BDIZC] = %d%d%d%d%d%d%d |",
			cpu.pc, cpu.a, cpu.x, cpu.y, cpu.sp, cpu.ps[0], cpu.ps[1], cpu.ps[2], cpu.ps[3], cpu.ps[4], cpu.ps[5], cpu.ps[6]);

	printf("\n");
}

void sbc(CPU *cpu, int operand, bool isOperandNegative) {
	if ((*cpu).a < 0) isOperandNegative = true;
	else isOperandNegative = false;
	(*cpu).a = (*cpu).a - operand - (1-(*cpu).ps[6]);
	if ((*cpu).a == 0) (*cpu).ps[5] = 1;
	if ((*cpu).a < 0) (*cpu).ps[0] = 1;
	if ((*cpu).a < -128) {
		(*cpu).ps[1] = 1;
		(*cpu).ps[6] = 0;
	} else {
		(*cpu).ps[1] = 0;
		(*cpu).ps[6] = 1;
	}
	(*cpu).pc++;
}

// void sta (CPU *cpu, int )

void LeCartucho(const char *arquivo){
	ifstream binario;
	binario.open(arquivo, ios::in | ios::binary | ios::ate);
	
	CPU cpu;
	HashMapTable memory;

	int operand;
	char immediate;
    char value;

    char zero_pg_addr;
    int absolute_addr;

    char result;

	bool isOperandNegative;
    bool is_IRQ_enabled = false;
    bool exit_emulation = false;

	streampos size;
	char * memblock;

	if(binario.is_open()){
		size = binario.tellg();
		memblock = new char [size];
		binario.seekg(0, ios::beg);
		binario.read(memblock, size);
		binario.close();
	}
	
	for(cpu.pc = 0; cpu.pc < size && !exit_emulation;){
		Instrucao instrucao = Decodifica(cpu.pc, memblock);
	
		switch(instrucao.opcode){
            //
            //ADC
            //
            //Immediate
            case(0x69):
                immediate = readNextByte(cpu.pc, memblock);

                result = cpu.a + immediate + (char)cpu.ps[C];

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Zero Page
            case(0x65):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)zero_pg_addr);

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(value > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Zero Page, X
            case(0x75):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)(zero_pg_addr + cpu.x));

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Absolute
            case(0x6d):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 3;

                break;

            //Absolute, X
            case(0x7d):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = memory.SearchKey((int)(absolute_addr + (int)cpu.x));

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 3;

                break;

            //Absolute Y
            case(0x79):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = memory.SearchKey((int)(absolute_addr + (int)cpu.y));

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 3;

                break;

            //(Indirect, X)
            case(0x61):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                zero_pg_addr += cpu.x;

                absolute_addr = memory.SearchKey((int)zero_pg_addr + 1);
                absolute_addr = absolute_addr << 8;
                absolute_addr = memory.SearchKey((int)zero_pg_addr);

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 2;

                break;
            //(Indirect), Y
            case(0x71):
                zero_pg_addr = readNextByte(cpu.pc, memblock);

                absolute_addr = memory.SearchKey((int)zero_pg_addr + 1);
                absolute_addr = absolute_addr << 8;
                absolute_addr = memory.SearchKey((int)zero_pg_addr);
                absolute_addr += (int)cpu.y;

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a + (char)cpu.ps[C] + value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                if(immediate > 0 && cpu.a > 0 && result < 0){

                    cpu.ps[C] = 1;
                    cpu.ps[N] = 1;
                }
                else{

                    cpu.ps[C] = 0;
                    cpu.ps[N] = 0;
                }

                cpu.a = result;
                cpu.pc += 2;

                break;

            //
            //AND
            //
            //Immediate
            case(0x29):
                immediate = readNextByte(cpu.pc, memblock);

                result = cpu.a & immediate ;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Zero Page
            case(0x25):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)zero_pg_addr);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Zero Page, X
            case(0x35):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)(zero_pg_addr + cpu.x));

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 2;

                break;

            //Absolute
            case(0x2d):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 3;

                break;

            //Absolute, X
            case(0x3d):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = memory.SearchKey((int)(absolute_addr + (int)cpu.x));

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 3;

                break;

            //Absolute, Y
            case(0x39):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = memory.SearchKey((int)(absolute_addr + (int)cpu.y));

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 3;

                break;

            //(Indirect, X)
            case(0x21):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                zero_pg_addr += cpu.x;

                absolute_addr = memory.SearchKey((int)zero_pg_addr + 1);
                absolute_addr = absolute_addr << 8;
                absolute_addr = memory.SearchKey((int)zero_pg_addr);

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 2;

                break;

            //(Indirect), Y
            case(0x31):
                zero_pg_addr = readNextByte(cpu.pc, memblock);

                absolute_addr = memory.SearchKey((int)zero_pg_addr + 1);
                absolute_addr = absolute_addr << 8;
                absolute_addr = memory.SearchKey((int)zero_pg_addr);
                absolute_addr += (int)cpu.y;

                value = (char)memory.SearchKey(absolute_addr);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 2;

                break;

            //
            //ASL
            //
            //Accumulator
            case(0x0A):
                cpu.ps[C] = cpu.a >> 6;

                result = cpu.a << 1;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.a = result;
                cpu.pc += 1;

                break;

            //Zero Page
            case(0x06):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)zero_pg_addr);

                cpu.ps[C] = value >> 6;
                result = value << 1;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                memory.Insert((int)zero_pg_addr, result);
                cpu.pc += 2;

                break;

            //Zero Page, X
            case(0x16):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)zero_pg_addr + cpu.x);

                cpu.ps[C] = value >> 6;
                result = value << 1;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                memory.Insert((int)zero_pg_addr + (int)cpu.x, result);
                cpu.pc += 2;

                break;

            //Absolute
            case(0x0e):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = (char)memory.SearchKey(absolute_addr);

                cpu.ps[C] = value >> 6;
                result = value << 1;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                memory.Insert(absolute_addr, result);
                cpu.pc += 3;

                break;

            //Absolute, X
            case(0x1e):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = (char)memory.SearchKey(absolute_addr + (int)cpu.x);

                cpu.ps[C] = value >> 6;
                result = value << 1;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                memory.Insert(absolute_addr + (int)cpu.x, result);
                cpu.pc += 3;

                break;

			// 
			//BCC
			// 
			case(0x90):
                if(cpu.ps[C] == 0)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BCS
			// 
			case(0xb0):
                if(cpu.ps[C] == 1)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BEQ
			// 
			case(0xf0):
                if(cpu.ps[Z] == 1)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BIT
			// 
            //Zero Page
            case(0x24):
                zero_pg_addr = readNextByte(cpu.pc, memblock);
                value = memory.SearchKey((int)zero_pg_addr);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[V] = (((result & 0x20) != 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.pc += 2;

                break;


            //Absolute
			case(0x2c):
                absolute_addr = (int)readNextByte(cpu.pc, memblock);
                absolute_addr = absolute_addr << 8;
                absolute_addr += (int)readNextByte(cpu.pc, memblock);

                value = (char)memory.SearchKey(absolute_addr + (int)cpu.x);

                result = cpu.a & value;

                cpu.ps[Z] = ((result == 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);
                cpu.ps[N] = ((result < 0) ? 1 : 0);

                cpu.pc += 3;

                break;

			// 
			//BMI
			// 
			case(0x30):
                if(cpu.ps[N] == 1)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BNE
			// 
			case(0xd0):
                if(cpu.ps[Z] == 0)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BPL
			// 
			case(0x10):
                if(cpu.ps[N] == 0)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BRK
			// 
			case(0x00):

                if(is_IRQ_enabled){
                    cpu.ps[B] = 1;

                    value = 0;

                    value += ((cpu.ps[C]) ? 1 : 0) << C;
                    value += ((cpu.ps[Z]) ? 1 : 0) << Z;
                    value += ((cpu.ps[I]) ? 1 : 0) << I;
                    value += ((cpu.ps[D]) ? 1 : 0) << D;
                    value += ((cpu.ps[B]) ? 1 : 0) << B;
                    value += 1 << (B+1);
                    value += ((cpu.ps[V]) ? 1 : 0) << V;
                    value += ((cpu.ps[N]) ? 1 : 0) << N;

                    memory.Insert(--(cpu.sp), (int)value);

                    absolute_addr = readNextByte((int)size - 1, memblock) << 8;
                    absolute_addr += readNextByte((int)size - 2, memblock);

                    cpu.pc = absolute_addr;
                }
                else
                    exit_emulation = true;

                break;

			// 
			//BVC
			// 
			case(0x50):
                if(cpu.ps[V] == 0)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//BVS
			// 
			case(0x70):
                if(cpu.ps[V] == 1)
                    cpu.pc += (int)readNextByte(cpu.pc, memblock);

                break;

			// 
			//CLC
			// 
			case(0x18):
                cpu.ps[C] = 0;
                break;

			// 
			//RTS
			case(96): 	//60 -- implied
				// cpu.pc = cpu.memStack.top();
				cpu.pc++;
				// cpu.memStack.pop();
				break;
			//SBC
			case(233): 	//e9 -- immediate
				operand = readNextByte(++cpu.pc, memblock);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(229): 	//e5 -- zero page
				operand = readNextByte(++cpu.pc, memblock);
				operand = memory.SearchKey(operand);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
 			case(245): 	//f5 -- zero page, x
				operand = readNextByte(++cpu.pc, memblock);
				operand = memory.SearchKey(operand+cpu.x);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(237): 	//ed -- absolute
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				operand = memory.SearchKey(operand);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(253): 	//fd -- absolute, x
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				operand = memory.SearchKey(operand+cpu.x);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(249): 	//f9 -- absolute, y
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				operand = memory.SearchKey(operand+cpu.y);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(225): 	//e1 -- (indirect, y)
				operand = readNextByte(++cpu.pc, memblock);
				operand = memory.SearchKey(operand+cpu.x);
				operand = memory.SearchKey(operand);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			case(241): 	//f1 -- (indirect), x
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				operand = memory.SearchKey(operand+cpu.y);
				operand = memory.SearchKey(operand);
				sbc(&cpu, operand, isOperandNegative);
				cpu.pc++;
				break;
			//SEC
			case(56):		//38 -- implied
				cpu.ps[6] = 1;
				cpu.pc++;
				break;
			// SED
			case(248):	//f8 -- implied
				cpu.ps[3] = 1;
				cpu.pc++;
				break;
			// SEI
			case(120):	//78 -- implied
				cpu.ps[4] = 1;
				cpu.pc++;
				break;
			// STA
			case(133): 	//85 -- zero page
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand, cpu.a);
				cpu.pc++;
				break;
			case(149): 	//95 -- zero page, x
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand+cpu.x, cpu.a);
				cpu.pc++;
				break;
			case(141): 	//8d -- absolute
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				memory.Insert(operand, cpu.a);
				cpu.pc++;
				break;
			case(157): 	//9d -- absolute, x
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				memory.Insert(operand+cpu.x, cpu.a);
				cpu.pc++;
				break;
			case(153): 	//99 -- absolute, y
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				memory.Insert(operand+cpu.y, cpu.a);
				cpu.pc++;
				break;
			case(129): 	//81 -- (indirect, x)
				operand = readNextByte(++cpu.pc, memblock);
				operand = memory.SearchKey(operand+cpu.x);
				memory.Insert(operand, cpu.a);
				cpu.pc++;
				break;
			case(145): 	//91 -- (indirect), y
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				operand = memory.SearchKey(operand+cpu.y);
				memory.Insert(operand, cpu.a);
				cpu.pc++;
				break;
			// STX
			case(134): 	//86 -- zero page
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand, cpu.x);
				cpu.pc++;
				break;
			case(150): 	//96 -- zero page, y
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand+cpu.y, cpu.x);
				cpu.pc++;
				break;
			case(142): 	//8e -- absolute
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				memory.Insert(operand, cpu.x);
				cpu.pc++;
				break;
			// STY
			case(132): 	//84 -- zero page
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand, cpu.y);
				cpu.pc++;
				break;
			case(148): 	//94 -- zero page, x
				operand = readNextByte(++cpu.pc, memblock);
				memory.Insert(operand+cpu.x, cpu.y);
				cpu.pc++;
				break;
			case(140): 	//8c -- absolute
				operand = readNextByte(++cpu.pc, memblock);
				operand += (readNextByte(++cpu.pc, memblock) << 8);
				memory.Insert(operand, cpu.y);
				cpu.pc++;
				break;
			// TAX
			case(170): 	//aa -- implied
				cpu.x = cpu.a;
				if (cpu.x == 0) cpu.ps[5] = 1;
				if (cpu.x < 0) cpu.ps[0] = 1;
				cpu.pc++;
				break;
			// TAY
			case(168): 	//a8 -- implied
				cpu.y = cpu.a;
				if (cpu.y == 0) cpu.ps[5] = 1;
				if (cpu.y < 0) cpu.ps[0] = 1;
				cpu.pc++;
				break;
			// TSX
			case(186): 	//ba -- implied
				cpu.x = cpu.sp;
				if (cpu.x == 0) cpu.ps[5] = 1;
				if (cpu.x < 0) cpu.ps[0] = 1;
				cpu.pc++;
				break;
			// TXA
			case(138): 	//8a -- implied
				cpu.a = cpu.x;
				if (cpu.a == 0) cpu.ps[5] = 1;
				if (cpu.a < 0) cpu.ps[0] = 1;
				cpu.pc++;
				break;
			// TXS
			case(154): 	//9a -- implied
				cpu.sp = cpu.x;
				cpu.pc++;
				break;
			// TYA
			case(152): 	//98 -- implied
				cpu.a = cpu.y;
				if (cpu.a == 0) cpu.ps[5] = 1;
				if (cpu.a < 0) cpu.ps[0] = 1;
				cpu.pc++;
				break;
			default:
				// printf("erro\n");
				cpu.pc++;
		}
		//log(cpu);
		//std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(int argc, const char *argv[]){
	const char *nomearquivo = argv[1];

	LeCartucho(nomearquivo);

	return 0;
}
