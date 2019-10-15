#include <bits/stdc++.h>

#include "headers/memory.hpp"
#include "headers/cpu.hpp"

using namespace std;

int main(int argc, const char *argv[]){

    // Passei a criacao do memblock pra ca, para que a
    // memoria e a cpu fossem "globais"
	const char *arquivo = argv[1];
	ifstream binario;

	binario.open(arquivo, ios::in | ios::binary | ios::ate);

	streampos size;
    char *memblock;

	if(binario.is_open()) {
		size = binario.tellg();
		memblock = new char [size];
		binario.seekg(0, ios::beg);
		binario.read(memblock, size);
	}
	else {
        printf("Error opening file :(\n");
        return 1;
    }

    // Create memory - RAM and ROM based on file
    Memory memory(memblock);

    // Initialize PC with the address for RESET label
    CPU cpu(memory.RESET_ADDR);
    binario.close();

    while(cpu.ExecuteNextInstruction(&memory));

	return 0;
}
