#include <ios>
#include <fstream>
#include <iostream>

#include "headers/addressbus.hpp"
#include "headers/cpu.hpp"
#include "headers/memory.hpp"
#include "headers/ppu.hpp"
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

    // Create memory - RAM and ROM based on file
    Memory memory(memblock);
    binario.close();

    // Initialize PC with the address for RESET label
    CPU cpu(memory.RESET_ADDR, memory.NMI_ADDR);

    std::vector<std::vector<uint8_t>> p_matrix(SCREEN_SIZE_X, std::vector<uint8_t>(SCREEN_SIZE_Y, 0));
    // Create PPU ({} is used to explicitly call default constructor)
    PPU ppu{};
	//ppu.WriteToRegister(PPUMASK_ADDR, 8);
	//ppu.Rendering(2251, &p_matrix);

    AddressBus addr_bus(&cpu, &memory, &ppu);

    cpu.SetAddressBus(&addr_bus);
    ppu.SetAddressBus(&addr_bus);

	tela(p_matrix);

    // Main cpu loop
    while(cpu.ExecuteNextInstruction()){
        ppu.Rendering(cpu.GetNumCycles(), &p_matrix);
		updateTela(p_matrix);
    }

	return 0;
}
