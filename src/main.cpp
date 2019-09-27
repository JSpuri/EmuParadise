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
	char opcode = 0;
	
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

void setFlagsCMP(int operand, char registrador, CPU *cpu){ // also used in CPX anc CPY
    cpu->ps[N] = ((registrador - operand) < 0);

    cpu->ps[Z] = ((registrador - operand) == 0);

    cpu->ps[C] = (registrador >= operand);
}

void setFlagsDEC(int operand, CPU *cpu){ // also used in DEX, DEY, INC, INX, INY
	cpu->ps[N] = (operand < 0);
	
	cpu->ps[Z] = (operand == 0);
}

void setFlagsEOR(char operand, CPU *cpu){
	cpu->ps[N] = (operand < 0);
	
	cpu->ps[Z] = (operand == 0);
}

void LeCartucho(const char *arquivo){
	ifstream binario;
	binario.open(arquivo, ios::in | ios::binary | ios::ate);
	
	CPU cpu;
	HashMapTable memory;
	int operand;
	bool isOperandNegative;

	streampos size;
	char * memblock;

	if(binario.is_open()){
		size = binario.tellg();
		memblock = new char [size];
		binario.seekg(0, ios::beg);
		binario.read(memblock, size);
		binario.close();
	}
	
	cpu.pc = 0;
	while(cpu.pc < size){
		Instrucao instrucao = Decodifica(cpu.pc, memblock);

	
		switch(instrucao.opcode){
			// 
			//RTS
			// 
			case(96): //60
				// cpu.pc = cpu.memStack.top();
				cpu.pc++;
				// cpu.memStack.pop();
				break;
			// 
			//SBC
			// 
			case(233): //e9
				operand = readNextByte(cpu.pc + 1, memblock);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - operand - (1-cpu.ps[6]);
				if (cpu.a == 0) cpu.ps[5] = 1;
				if (cpu.a < 0) cpu.ps[0] = 1;
				if (cpu.a < -128) {
					cpu.ps[1] = 1;
					cpu.ps[6] = 0;
				} else {
					cpu.ps[1] = 0;
					cpu.ps[6] = 1;
				}
				cpu.pc+=2;
				break;
			case(229): //e5
				operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - operand - (1-cpu.ps[6]);
				if (cpu.a == 0) cpu.ps[5] = 1;
				if (cpu.a < 0) cpu.ps[0] = 1;
				if (cpu.a < -128) {
					cpu.ps[1] = 1;
					cpu.ps[6] = 0;
				} else {
					cpu.ps[1] = 0;
					cpu.ps[6] = 1;
				}
				cpu.pc+=2;
				break;
 			case(245): //f5
				operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand+cpu.x);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - operand - (1-cpu.ps[6]);
				if (cpu.a == 0) cpu.ps[5] = 1;
				if (cpu.a < 0) cpu.ps[0] = 1;
				if (cpu.a < -128) {
					cpu.ps[1] = 1;
					cpu.ps[6] = 0;
				} else {
					cpu.ps[1] = 0;
					cpu.ps[6] = 1;
				}
				cpu.pc+=2;
				break;
			case(237): //ed
			case(253): //fd
			case(249): //f9
				cpu.pc+=3;
				break;
			case(225): //e1
			case(241): //f1
				cpu.pc+=2;
				break;
			// 
			//SEC
			// 
			case(56): //38
				cpu.pc++;
				break;
			// 
			// SED
			// 
			case(248): //f8
				cpu.pc++;
				break;
			// 
			// SEI
			// 
			case(120): //78
				cpu.pc++;
				break;
			// 
			// STA
			// 
			case(133): //85
			case(149): //95
				cpu.pc+=2;
				break;
			case(141): //8d
			case(157): //9d
			case(153): //99
				cpu.pc+=3;
				break;
			case(129): //81
			case(145): //91
				cpu.pc+=2;
				break;
			// 
			// STX
			// 
			case(134): //86
			case(150): //96
				cpu.pc+=2;
				break;
			case(142): //8e
				cpu.pc+=3;
				break;
			// 
			// STY
			// 
			case(132): //84
			case(148): //94
				cpu.pc+=2;
				break;
			case(140): //8c
				cpu.pc+=3;
				break;
			// 
			// TAX
			// 
			case(170): //aa
				cpu.pc++;
				break;
			// 
			// TAY
			// 
			case(168): //a8
				cpu.pc++;
				break;
			// 
			// TSX
			// 
			case(186): //ba
				cpu.pc++;
				break;
			// 
			// TXA
			// 
			case(138): //8a
				cpu.pc++;
				break;
			// 
			// TXS
			// 
			case(154): //9a
				cpu.pc++;
				break;
			// 
			// TYA
			// 
			case(152): //98
				cpu.pc++;
				break;
			case(0x58): // CLI
			    cpu.ps[5] = 0;
				cpu.pc++;
			    break;
			case(0xB8): // CLV
			    cpu.ps[1] = 0;
				cpu.pc++;
				break;
			case(0xD8): // CLD
			    cpu.ps[D] = 0;
				cpu.pc++;
				break;
			///	
			/// CMP
			///
			case(0xC9): //immediate
			    operand = readNextByte(cpu.pc + 1, memblock);
				
				setFlagsCMP(operand, cpu.a, &cpu);
				
			    cpu.pc += 2;
				
			    break;
			case(0xC5): //zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				 
				setFlagsCMP(operand, cpu.a, &cpu);
				
				cpu.pc += 2;
			    break;
			case(0xD5): //zero page + x
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand + cpu.x);
				
				setFlagsCMP(operand, cpu.a, &cpu);
				
			    cpu.pc += 2;
			    break;
			case(0xCD): //absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			    operand = memory.SearchKey(operand);
			   
			    setFlagsCMP(operand, cpu.a, &cpu);
			   
			    cpu.pc += 3;
			    break;
				
			case(0xDD): //absolute + x
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				operand = memory.SearchKey(operand + cpu.x);
				
				setFlagsCMP(operand, cpu.a, &cpu);
				
			    cpu.pc += 3;
			    break;
			case(0xD9): //absolute + y
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				operand = memory.SearchKey(operand + cpu.y);
			
			    setFlagsCMP(operand, cpu.a, &cpu);
				
			    cpu.pc += 3;
			    break;
				
			case(0xC1): // (indirect, x)
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand + cpu.x);
				operand = memory.SearchKey(operand);
				
				setFlagsCMP(operand, cpu.a, &cpu);
			
			    cpu.pc += 2;
			    break;
			case(0xD1): // (indirect), y
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				operand = memory.SearchKey(operand + cpu.y);
				
				setFlagsCMP(operand, cpu.a, &cpu);
				
				cpu.pc += 2;
				break;
		    ///	
			/// CPX
			///
			case(0xE0): //immediate
			    operand = readNextByte(cpu.pc + 1, memblock);
				
				setFlagsCMP(operand, cpu.x, &cpu);
				
			    cpu.pc += 2;
				
			    break;
			case(0xE4): //zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				 
				setFlagsCMP(operand, cpu.x, &cpu);
				
				cpu.pc += 2;
			    break;
			case(0xEC): //absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			    operand = memory.SearchKey(operand);
			   
			    setFlagsCMP(operand, cpu.x, &cpu);
			   
			    cpu.pc += 3;
			    break;
		    ///	
			/// CPY
			///
			case(0xC0): //immediate
			    operand = readNextByte(cpu.pc + 1, memblock);
				
				setFlagsCMP(operand, cpu.y, &cpu);
				
			    cpu.pc += 2;
				
			    break;
			case(0xC4): //zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				 
				setFlagsCMP(operand, cpu.y, &cpu);
				
				cpu.pc += 2;
			    break;
			case(0xCC): //absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			    operand = memory.SearchKey(operand);
			   
			    setFlagsCMP(operand, cpu.y, &cpu);
			   
			    cpu.pc += 3;
			    break;
				
			///
            /// DEC			
			///	
			case(0xC6): // zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				memblock[operand] -= 1;
				
				setFlagsDEC(memblock[operand], &cpu);
				
				cpu.pc += 2;
			    break;
			case(0xD6): // zero page + x
			    operand = readNextByte(cpu.pc + 1, memblock);
				memblock[operand + cpu.x] -= 1;
				
				setFlagsDEC(memblock[operand + cpu.x], &cpu);
				
			    cpu.pc += 2;
			    break;			
			case(0xCE): // absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				
				memblock[operand] -= 1;
				
				setFlagsDEC(memblock[operand], &cpu);
				
			    cpu.pc += 3;
			    break;
			case(0xDE): // absolute + x
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			
			    memblock[operand + cpu.x] -= 1;
			
			    setFlagsDEC(memblock[operand + cpu.x], &cpu);
			
			    cpu.pc += 3;
			    break;
			///	
			/// DEX
            ///			
			case(0xCA):
				cpu.x--;
				
				setFlagsDEC(cpu.x, &cpu);
				
				cpu.pc++;
			    break;
			///
            /// DEY			
			///
            case(0x88):
               cpu.y--;

               setFlagsDEC(cpu.y, &cpu);

               cpu.pc++;
               break;
            ///
            /// EOR
			///
			case(0x49): //immediate
			    operand = readNextByte(cpu.pc + 1, memblock);
				
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
			    cpu.pc += 2;
				
			    break;
			case(0x45): //zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				 
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
				cpu.pc += 2;
			    break;
			case(0x55): //zero page + x
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand + cpu.x);
				
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
			    cpu.pc += 2;
			    break;
			case(0x4D): //absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			    operand = memory.SearchKey(operand);
			   
			    cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
			   
			    cpu.pc += 3;
			    break;
				
			case(0x5D): //absolute + x
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				operand = memory.SearchKey(operand + cpu.x);
				
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
			    cpu.pc += 3;
			    break;
			case(0x59): //absolute + y
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				operand = memory.SearchKey(operand + cpu.y);
			
			    cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
			    cpu.pc += 3;
			    break;
				
			case(0x41): // (indirect, x)
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand + cpu.x);
				operand = memory.SearchKey(operand);
				
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
			
			    cpu.pc += 2;
			    break;
			case(0x51): // (indirect), y
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand = memory.SearchKey(operand);
				operand = memory.SearchKey(operand + cpu.y);
				
				cpu.a = cpu.a ^ operand;
				
				setFlagsEOR(operand, &cpu);
				
				cpu.pc += 2;
				break;				
			///
            /// DEC			
			///	
			case(0xE6): // zero page
			    operand = readNextByte(cpu.pc + 1, memblock);
				memblock[operand] += 1;
				
				setFlagsDEC(memblock[operand], &cpu);
				
				cpu.pc += 2;
			    break;
			case(0xF6): // zero page + x
			    operand = readNextByte(cpu.pc + 1, memblock);
				memblock[operand + cpu.x] += 1;
				
				setFlagsDEC(memblock[operand + cpu.x], &cpu);
				
			    cpu.pc += 2;
			    break;			
			case(0xEE): // absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				
				memblock[operand] += 1;
				
				setFlagsDEC(memblock[operand], &cpu);
				
			    cpu.pc += 3;
			    break;
			case(0xFE): // absolute + x
			    operand = readNextByte(cpu.pc + 1, memblock);
			    operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
			
			    memblock[operand + cpu.x] += 1;
			
			    setFlagsDEC(memblock[operand + cpu.x], &cpu);
			
			    cpu.pc += 3;
			    break;
			///	
			/// INX
            ///			
			case(0xE8):
				cpu.x++;
				
				setFlagsDEC(cpu.x, &cpu);
				
				cpu.pc++;
			    break;
			///
            /// INY			
			///
            case(0xC8):
                cpu.y++;

                setFlagsDEC(cpu.y, &cpu);

                cpu.pc++;
                break;
			///	
			/// JMP	
			///
            case(0x4C): // absolute
			    operand = readNextByte(cpu.pc + 1, memblock);
				operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
				cpu.pc = operand;
				
                cpu.pc += 3;
                break;
            case(0x6C): // indirect
                operand = readNextByte(cpu.pc + 1, memblock);
                operand += (readNextByte(cpu.pc + 2, memblock)) << 8;
                cpu.pc = memory.SearchKey(operand);

                cpu.pc += 3;
			    break;
			default:
			    printf("erro\n");
			
			    cpu.pc++;
			    break;
		}
		log(cpu);
		//std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(int argc, const char *argv[]){
	const char *nomearquivo = argv[1];

	LeCartucho(nomearquivo);

	return 0;
}
