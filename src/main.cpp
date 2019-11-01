#include <ios>
#include <fstream>
#include <iostream>

#include "headers/addressbus.hpp"
#include "headers/cpu.hpp"
#include "headers/memory.hpp"
#include "headers/ppu.hpp"

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

    // Create memory - RAM and ROM based on file
    Memory memory(memblock);
    binario.close();

    // Initialize PC with the address for RESET label
    CPU cpu(memory.RESET_ADDR, memory.NMI_ADDR);

    // Create PPU ({} is used to explicitly call default constructor)
    PPU ppu{};

    AddressBus addr_bus(&cpu, &memory, &ppu);

    cpu.SetAddressBus(&addr_bus);
    ppu.SetAddressBus(&addr_bus);

    
    // Main cpu loop
    while(cpu.ExecuteNextInstruction()){
        ppu.Rendering(cpu.GetNumCycles());
    }

	return 0;
}
