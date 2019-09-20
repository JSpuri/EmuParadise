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

int readNextByte(int pc, char *memblock) {
	int byte = 0;

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
	
	for(cpu.pc = 0; cpu.pc < size;){
		Instrucao instrucao = Decodifica(cpu.pc, memblock);
	
		switch(instrucao.opcode){
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
		log(cpu);
		//std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(int argc, const char *argv[]){
	const char *nomearquivo = argv[1];

	LeCartucho(nomearquivo);

	return 0;
}
