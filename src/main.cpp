#include <stdlib.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
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

void log(){
	
}

void LeCartucho(const char *arquivo){
	ifstream binario;
	binario.open(arquivo, ios::in | ios::binary | ios::ate);
	
	int pc = 0;
	streampos size;
	char * memblock;

	if(binario.is_open()){
		size = binario.tellg();
		memblock = new char [size];
		binario.seekg(0, ios::beg);
		binario.read(memblock, size);
		binario.close();
	}
	
	for(pc = 0; pc < size; pc++){
		Instrucao instrucao = Decodifica(pc, memblock);

	
	switch(instrucao.opcode){
		// 
		//RTS
		// 
		case(96): //60
			cout << "Instrução RTS" << endl;
			pc++;
			break;
		// 
		//SBC
		// 
		case(233): //e9
		case(229): //e5
		case(245): //f5
			cout << "Instrução SBC Zero Page" << endl;
			pc+=2;
			break;
		case(237): //ed
		case(253): //fd
		case(249): //f9
			cout << "Instrução SBC Absolute" << endl;
			pc+=3;
			break;
		case(225): //e1
		case(241): //f1
			cout << "Instrução SBC Indirect" << endl;
			pc+=2;
			break;
		// 
		//SEC
		// 
		case(56): //38
			cout << "Instrução SEC" << endl;
			pc++;
			break;
		// 
		// SED
		// 
		case(248): //f8
			cout << "Instrução SED" << endl;
			pc++;
			break;
		// 
		// SEI
		// 
		case(120): //78
			cout << "Instrução SEI" << endl;
			pc++;
			break;
		// 
		// STA
		// 
		case(133): //85
		case(149): //95
			cout << "Instrução STA Zero Page" << endl;
			pc+=2;
			break;
		case(141): //8d
		case(157): //9d
		case(153): //99
			cout << "Instrução STA Absolute" << endl;
			pc+=3;
			break;
		case(129): //81
		case(145): //91
			cout << "Instrução STA Indirect" << endl;
			pc+=2;
			break;
		// 
		// STX
		// 
		case(134): //86
		case(150): //96
			cout << "Instrução STX Zero Page" << endl;
			pc+=2;
			break;
		case(142): //8e
			cout << "Instrução STX Absolute" << endl;
			pc+=3;
			break;
		// 
		// STY
		// 
		case(132): //84
		case(148): //94
			cout << "Instrução STY Zero Page" << endl;
			pc+=2;
			break;
		case(140): //8c
			cout << "Instrução STY Absolute" << endl;
			pc+=3;
			break;
		// 
		// TAX
		// 
		case(170): //aa
			cout << "Instrução TAX" << endl;
			pc++;
			break;
		// 
		// TAY
		// 
		case(168): //a8
			cout << "Instrução TAY" << endl;
			pc++;
			break;
		// 
		// TSX
		// 
		case(186): //ba
			cout << "Instrução TSX" << endl;
			pc++;
			break;
		// 
		// TXA
		// 
		case(138): //8a
			cout << "Instrução TXA" << endl;
			pc++;
			break;
		// 
		// TXS
		// 
		case(154): //9a
			cout << "Instrução TXS" << endl;
			pc++;
			break;
		// 
		// TYA
		// 
		case(152): //98
			cout << "Instrução TYA" << endl;
			pc++;
			break;
		default:
			printf("erro\n");
			pc++;
	}
		log();
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(int argc, const char *argv[]){
	const char *nomearquivo = argv[1];

	LeCartucho(nomearquivo);

	return 0;
}
