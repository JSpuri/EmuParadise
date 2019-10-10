#include "headers/opcodes.hpp"
#include "headers/cpu.hpp"
#include "headers/memory.hpp"
#include "../common/constants.hpp"

Opcodes::Opcodes(Memory *memory, CPU *cpu) {
	this->memory = memory;
	this->cpu = cpu;
}

int Opcodes::ADC(int mode) {
    int8_t current_bit_a, current_bit_num, current_bit_result;
	int8_t aux_a, aux_num, aux_result;

	int num = 0;

    int8_t result = cpu->a + num + cpu->ps[C];

	aux_a = cpu->a;
	aux_num = num;
	aux_result = result;

	uint8_t u_result = (uint8_t) cpu->a + num;
	if(u_result < (uint8_t) cpu->a || u_result < (uint8_t) num){
		cpu->ps[C] = 1;
	}
	else{
		cpu->ps[C] = 0;
	}

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[N] = ((result < 0) ? 1 : 0);

    if(((num > 0) && (cpu->a > 0) && (result <= 0)) || ((num < 0) && (cpu->a < 0) && (result >= 0))){
				cpu->ps[V] = 1;
			}
    else if((num + cpu->a == 0x7f) && (cpu->ps[C] == 1)){
		cpu->ps[V] = 1;
	}
    else{
        cpu->ps[V] = 0;
	}

    cpu->a = result;
}