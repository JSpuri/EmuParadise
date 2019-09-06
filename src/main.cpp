#include <stdlib.h>
#include <cstdio>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

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
	int pc = 0;
    while(1){
        Instrucao instrucao;

		instrucao = Decodifica(pc);
		
		switch(instrucao.opcode){
			case 0:
			    break;
		    case 1:
			    break;
			default:
			    printf("erro\n");
		}
		log();
		std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}

int main(int argc, const char *argv[]){
    const char *arquivo = argv[1];

    LeCartucho(arquivo);

    return 0;
}
