#include <ios>
#include <fstream>
#include <iostream>

#include "headers/addressbus.hpp"
#include "headers/cpu.hpp"
#include "headers/screen.hpp"

using namespace std;

int main(int argc, const char *argv[]){

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

    // Initialize PC with the address for RESET label
    CPU cpu{};

    // Create memory - RAM and ROM based on file
    AddressBus addr_bus(memblock, &cpu);
    binario.close();

    cpu.SetAddressBus(&addr_bus);

	// tela();

    // Main cpu loop
    while(addr_bus.Clock());

	return 0;
}
