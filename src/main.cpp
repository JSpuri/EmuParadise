#include <stdlib.h>
#include <cstdio>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
using namespace std;


struct Instrucao {
  int opcode;
};

Instrucao Decodifica(int pc){
	Instrucao instrucao;
	instrucao.opcode = 2;
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
        Instrucao instrucao;

		instrucao = Decodifica(pc);
		
		for(int i = 0; i < 8; i++) {
            printf("%d", !!((memblock[pc] << i) & 0x80));
        }
        printf("\n");
		
		switch(instrucao.opcode){
			default:
			    printf("erro\n");
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
