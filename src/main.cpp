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

void LeCartucho(const char *arquivo){
	ifstream binario;
	binario.open(arquivo, ios::in | ios::binary | ios::ate);
	
	CPU cpu;
	HashMapTable memory;
	int immediate;
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
	
	for(cpu.pc = 0; cpu.pc < size; cpu.pc++){
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
				immediate = readNextByte(cpu.pc, memblock);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - immediate - (1-cpu.ps[6]);
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
				immediate = readNextByte(cpu.pc, memblock);
				immediate = memory.SearchKey(immediate);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - immediate - (1-cpu.ps[6]);
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
				immediate = readNextByte(cpu.pc, memblock);
				immediate = memory.SearchKey(immediate+cpu.x);
				if (cpu.a < 0) isOperandNegative = true;
				else isOperandNegative = false;
				cpu.a = cpu.a - immediate - (1-cpu.ps[6]);
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
			default:
				printf("erro\n");
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
