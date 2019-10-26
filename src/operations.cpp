#include "headers/operations.hpp"

#include <bits/stdc++.h>

#include "common/constants.hpp"

// These are the functions that will be used to create
// Instructions objects. Their addresses will be passed
// to the operation function wrapper in said constructor.
//
// Each instruction from 6502 must be a function, as declared
// in ./headers/operations.hpp
//
// To port the current implementation of all instructions,
// *cpu and *memory were kept as arguments


void setFlagsCMP(int8_t operand, int8_t registrador, CPU *cpu){ // also used in CPX anc CPY
    cpu->ps[N] = ((registrador - operand) < 0);

    cpu->ps[Z] = ((registrador - operand) == 0);

	if(abs(registrador) >= abs(operand))
		cpu->ps[C] = 1;
	else
		cpu->ps[C] = 0;
    //cpu->ps[C] = (registrador >= operand);
}

void setFlagsDEC(int8_t operand, CPU *cpu){ // also used in DEX, DEY, INC, INX, INY
	cpu->ps[N] = (operand < 0);

	cpu->ps[Z] = (operand == 0);
}

void setFlagsEOR(int8_t operand, CPU *cpu){
	cpu->ps[N] = (operand < 0);

	cpu->ps[Z] = (operand == 0);
}

// Executes a sum operation between cpu->a and num, storing
// the result in cpu->a, setting flags accordingly
void adc_aux(CPU *cpu, int8_t num) {
	int8_t current_bit_a, current_bit_num, current_bit_result;
	int8_t aux_a, aux_num, aux_result;

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

void ADC(int mode, CPU *cpu) {

	int8_t num = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

    adc_aux(cpu, num);
}

void AND(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

    int8_t result = cpu->a & value;

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[N] = ((result < 0) ? 1 : 0);

    cpu->a = result;
}

void ASL(int mode, CPU *cpu) {

    int8_t result;

    if(mode == M_ACCUMULATOR){

        cpu->ps[C] = (cpu->a & 0x80) >> 7; 
        result = cpu->a << 1;

        cpu->a = result;
    }
    else{

        int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
        cpu->ps[C] = (value & 0x80) >> 7;
        result = value << 1;

        uint16_t target_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
        cpu->WriteTo(target_addr, result);
    }

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[N] = ((result < 0) ? 1 : 0);
}

void BCC(int mode, CPU *cpu) {
    if(cpu->ps[C] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BCS(int mode, CPU *cpu) {
    if(cpu->ps[C] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BEQ(int mode, CPU *cpu) {
    if(cpu->ps[Z] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BIT(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    int8_t result = cpu->a & value;

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[V] = (((value & 0x40) != 0) ? 1 : 0);
    cpu->ps[N] = ((value < 0) ? 1 : 0);
}

void BMI(int mode, CPU *cpu) {
    if(cpu->ps[N] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BNE(int mode, CPU *cpu) {
    if(cpu->ps[Z] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BPL(int mode, CPU *cpu) {
    if(cpu->ps[N] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BRK(int mode, CPU *cpu) {
}

void BVC(int mode, CPU *cpu) {
    if(cpu->ps[V] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void BVS(int mode, CPU *cpu) {
    if(cpu->ps[V] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 1;
}

void CLC(int mode, CPU *cpu) {
    cpu->ps[C] = 0;
}

void CLD(int mode, CPU *cpu) {
    cpu->ps[D] = 0;
}

void CLI(int mode, CPU *cpu) {
    cpu->ps[I] = 0;
}

void CLV(int mode, CPU *cpu) {
    cpu->ps[V] = 0;
}

void CMP(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    setFlagsCMP(value, cpu->a, cpu);
}

void CPX(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    setFlagsCMP(value, cpu->x, cpu);
}

void CPY(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    setFlagsCMP(value, cpu->y, cpu);
}

void DEC(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    value -= 1;

    setFlagsDEC(value, cpu);

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->WriteTo(addr, value);

}

void DEX(int mode, CPU *cpu) {

    cpu->x -= 1;
    setFlagsDEC(cpu->x, cpu);
}

void DEY(int mode, CPU *cpu) {

    cpu->y -= 1;
    setFlagsDEC(cpu->y, cpu);
}

void EOR(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    cpu->a = cpu->a ^ value;

    setFlagsEOR(cpu->a, cpu);
}

void INC(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    value += 1;

    setFlagsDEC(value, cpu);

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->WriteTo(addr, value);

}

void INX(int mode, CPU *cpu) {

    cpu->x += 1;
    setFlagsDEC(cpu->x, cpu);
}

void INY(int mode, CPU *cpu) {

    cpu->y += 1;
    setFlagsDEC(cpu->y, cpu);
}

void JMP(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->pc = addr;
}

void JSR(int mode, CPU *cpu) {
    
    uint16_t absolute_addr = (cpu->pc) + 3;
    
    cpu->WriteTo(0x0100 + (cpu->sp)--, absolute_addr >> 8);
    cpu->WriteTo(0x0100 + (cpu->sp)--, absolute_addr & 0xFF);

    absolute_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->pc = absolute_addr;

}

void LDA(int mode, CPU *cpu) {

    cpu->a = cpu->ResolveOPArgWord(mode, cpu->pc +1);

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void LDX(int mode, CPU *cpu) {

    cpu->x = cpu->ResolveOPArgWord(mode, cpu->pc +1);

    if(cpu->x == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void LDY(int mode, CPU *cpu) {

    cpu->y = cpu->ResolveOPArgWord(mode, cpu->pc +1);

    if(cpu->y == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}
void LSR(int mode, CPU *cpu) {

    if(mode == M_ACCUMULATOR){

        cpu->ps[0] = 0;					//always positive
        if((cpu->a & 0x01) == 0x01)		//check carry
            cpu->ps[7] = 1;
        else
            cpu->ps[7] = 0;

        cpu->a = cpu->a >> 1;
        cpu->a = cpu->a & 0x7f; 		//mask and 0b01111111

        if(cpu->a == 0x00)			 	//check zero
            cpu->ps[6] = 1;
        else
            cpu->ps[6] = 0;
    }
    else{

        int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

        cpu->ps[0] = 0;					//always positive
        if((value & 0x01) == 0x01)		//check carry
            cpu->ps[7] = 1;
        else
            cpu->ps[7] = 0;

        value = value >> 1;
        value = value & 0x7f; 		//mask and 0b01111111

        if(value == 0x00)			 	//check zero
            cpu->ps[6] = 1;
        else
            cpu->ps[6] = 0;

        uint16_t target_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
        cpu->WriteTo(target_addr, value);
    }
}

void NOP(int mode, CPU *cpu) {
}

void ORA(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    cpu->a = cpu->a | value;

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void PHA(int mode, CPU *cpu) {
    cpu->WriteTo(0x0100 + (cpu->sp-- & 0xFF), cpu->a);
}

void PHP(int mode, CPU *cpu) {
    
    cpu->ps[2] = 1;
    cpu->ps[3] = 1;
    int8_t aux = cpu->ps[0]*pow(2,7);

    for(int j = 1; j < 8; j++)
        aux += cpu->ps[j]*pow(2,7-j);

    cpu->WriteTo(0x0100 + (cpu->sp-- & 0xFF), aux);

}

void PLA(int mode, CPU *cpu) {

    cpu->a = cpu->ReadFrom((0x0100 + (++cpu->sp & 0xFF)));

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void PLP(int mode, CPU *cpu) {

    int8_t aux = cpu->ReadFrom((0x0100 + (++cpu->sp & 0xFF)));

    for(int i = 0; i < 8; i++) {

        if(i != 2 && i != 3){

            if((aux & 0x80) == 0x80)
                cpu->ps[i] = 1;
            else
                cpu->ps[i] = 0;
        }

        aux = aux << 1;
    }
}

void ROL(int mode, CPU *cpu) {

    if(mode == M_ACCUMULATOR){
        int8_t aux = cpu->a;
        cpu->a = (cpu->a << 1);

        if(cpu->ps[7] == 1)				//if previous carry was 1
            cpu->a = (cpu->a | 0x01);

        if(cpu->a == 0)
            cpu->ps[6] = 1;					//verify zero
        else
            cpu->ps[6] = 0;

        if((cpu->a & 0x80) == 0x80)
            cpu->ps[0] = 1;     //verify neg
        else
            cpu->ps[0] = 0;

        if((aux & 0x80) == 0x80)   //update carry
            cpu->ps[7] = 1;
        else
            cpu->ps[7] = 0;
    }

    else{

        int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
        int8_t aux = value;

        value = (value << 1);

        if(cpu->ps[7] == 1)
            value = (value | 0x01);

        if (value == 0)
            cpu->ps[Z] = 1;
        else cpu->ps[Z] = 0;

        if((value & 0x80) == 0x80)
            cpu->ps[0] = 1;
        else
            cpu->ps[0] = 0;

        if((aux & 0x80) == 0x80)
            cpu->ps[7] = 1;
        else
            cpu->ps[7] = 0;

        uint16_t target_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
        cpu->WriteTo(target_addr, value);
    }
}

void ROR(int mode, CPU *cpu) {

    if(mode == M_ACCUMULATOR){

        int8_t aux = cpu->a;

        cpu->a = (cpu->a >> 1);
        cpu->a = cpu->a & 0x7f; 		//mask and 0b01111111

        if(cpu->ps[C] == 1) {
            cpu->a = (cpu->a | 0x80);
            cpu->ps[C] = aux & 0x01;					//is negative
        }

        if(cpu->a == 0)
            cpu->ps[Z] = 1;					//verify zero
        else
            cpu->ps[Z] = 0;

        if((cpu->a & 0x80) == 0x80)
            cpu->ps[N] = 1;     //verify neg
        else
            cpu->ps[N] = 0;

        if((aux & 0x01) == 0x01)   //update carry
            cpu->ps[C] = 1;
        else
            cpu->ps[C] = 0;
    }

    else{

        int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
        int8_t aux = value;

        value = (value >> 1);
        value = value & 0x7f; 		//mask and 0b01111111

        if(cpu->ps[C] == 1)
            value = (value | 0x80);

        if (value == 0)
            cpu->ps[Z] = 1;
        else cpu->ps[Z] = 0;

        if((value & 0x80) == 0x80)
            cpu->ps[0] = 1;
        else
            cpu->ps[0] = 0;

        if((aux & 0x01) == 0x01)
            cpu->ps[7] = 1;
        else
            cpu->ps[7] = 0;

        uint16_t target_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
        cpu->WriteTo(target_addr, value);
    }
}

void RTI(int mode, CPU *cpu) {

    int8_t aux = cpu->ReadFrom(0x0100 + (++cpu->sp));

    for(int i = 0; i < 8; i++) {

        if((aux & 0x01) == 0x01)
            cpu->ps[i] = 1;
        else
            cpu->ps[i] = 0;
        aux = aux >> 1;
        aux = aux & 0x7f; 		//mask and 0b01111111

    }

    cpu->pc = cpu->ReadFrom((0x0100 + (++cpu->sp)) & 0xFF);
    cpu->pc += cpu->ReadFrom(0x0100 + (++cpu->sp)) << 8 ;
}

void RTS(int mode, CPU *cpu) {

    uint16_t absolute_addr = cpu->ReadFrom(0x0100 + (++cpu->sp));
    absolute_addr += cpu->ReadFrom(0x0100 + (++cpu->sp)) << 8;

    cpu->pc = absolute_addr;

}

void SBC(int mode, CPU *cpu) { 

    int8_t operand = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

	int8_t op1, op2, result;
	result = cpu->a - operand;
	op1 = cpu->a;
	op2 = operand;

	int8_t num = (operand ^ 0xff) + 0x01;
	adc_aux(cpu, num);
	cpu->a -= 1;

	if((op1 >= 0 && op2 > 0 && result < 0))
		cpu->ps[C] = 1;
	else
		cpu->ps[C] = 0;
}

void SEC(int mode, CPU *cpu) {
    cpu->ps[C] = 1;
}

void SED(int mode, CPU *cpu) {
    cpu->ps[D] = 1;
}

void SEI(int mode, CPU *cpu) {
    cpu->ps[I] = 1;
}

void STA(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->WriteTo(addr, cpu->a);
}

void STX(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->WriteTo(addr, cpu->x);
}

void STY(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    cpu->WriteTo(addr, cpu->y);
}

void TAX(int mode, CPU *cpu) {

    cpu->x = cpu->a;
    if (cpu->x == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->x < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TAY(int mode, CPU *cpu) {

    cpu->y = cpu->a;
    if (cpu->y == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->y < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TSX(int mode, CPU *cpu) {

    cpu->x = cpu->sp;
    if (cpu->x == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->x < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TXA(int mode, CPU *cpu) {

    cpu->a = cpu->x;
    if (cpu->a == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->a < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TXS(int mode, CPU *cpu) {
    cpu->sp = cpu->x;
}

void TYA(int mode, CPU *cpu) {

    cpu->a = cpu->y;
    if (cpu->a == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->a < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

// ===========================================================================================
// A IMPLEMENTACAO DE CADA INSTRUCAO DENTRO DOS CASES DEVE SER PASSADA PARA FUNCOES, COMO
// EM ADC E SBC
// ===========================================================================================
/*
void readGame(Memory *memory, CPU *cpu) {

    uint8_t opcode;

	int8_t immediate;
    int8_t value;
	int8_t aux;

    uint8_t zero_pg_addr;
    uint16_t absolute_addr;

    int8_t result;


    bool is_IRQ_enabled = false;
    bool exit_emulation = false;


    // a leitura acaba quando o programa encontra um BRK E a interrupcao
    // IRQ esta desabilitada
	while(!exit_emulation) {
		opcode = memory->read(cpu->pc);
		switch(opcode){

			//ADC
			case(0x69): //Immediate
				immediate = memory->read(++(cpu->pc));

				adc(cpu, immediate);
				(cpu->pc)++;

				break;
			case(0x65): //Zero Page
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x75): //Zero Page, X
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr + cpu->x);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x6d): //Absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read(absolute_addr);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x7d): //Absolute, X
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read(absolute_addr + cpu->x);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x79): //Absolute Y
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read((absolute_addr + cpu->y));

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x61): //(Indirect, X)
				zero_pg_addr = memory->read(++(cpu->pc));
				zero_pg_addr += cpu->x;

				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;

				value = memory->read(absolute_addr);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x71): //(Indirect), Y
				zero_pg_addr = memory->read(++(cpu->pc));

				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;
				absolute_addr += cpu->y;

				value = memory->read(absolute_addr);

				adc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			//AND
			case(0x29): //Immediate
				immediate = memory->read(++(cpu->pc));

				result = cpu->a & immediate ;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
				(cpu->pc)++;

				break;
			case(0x25): //Zero Page
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x35): //Zero Page, X
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read((zero_pg_addr + cpu->x));

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x2d): //Absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = (char)memory->read(absolute_addr);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x3d): //Absolute, X
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read((absolute_addr + cpu->x));

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x39): //Absolute, Y
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read((absolute_addr + cpu->y));

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x21): //(Indirect, X)
				zero_pg_addr = memory->read(++(cpu->pc));
				zero_pg_addr += cpu->x;

				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;

				value = memory->read(absolute_addr);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x31): //(Indirect), Y
				zero_pg_addr = memory->read(++(cpu->pc));

				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;

				value = memory->read(absolute_addr + cpu->y);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				cpu->a = result;
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			//ASL
			case(0x0A): //Accumulator
				cpu->ps[c] = (cpu->a & 0x80) >> 7;

				result = cpu->a << 1;

				cpu->ps[z] = ((result == 0) ? 1 : 0);
				cpu->ps[n] = ((result < 0) ? 1 : 0);

				cpu->a = result;
				(cpu->pc)++;

				break;
			case(0x06): //Zero Page
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				cpu->ps[C] = (value & 0x80) >> 7;
				result = value << 1;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				memory->write(zero_pg_addr, result);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x16): //Zero Page, X
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr + cpu->x);

				cpu->ps[C] = (value & 0x80) >> 7;
				result = value << 1;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				memory->write(zero_pg_addr + cpu->x, result);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x0e): //Absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read(absolute_addr);

				cpu->ps[C] = (value & 0x80) >> 7;
				result = value << 1;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				memory->write(absolute_addr, result);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x1e): //Absolute, X
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read(absolute_addr + cpu->x);

				cpu->ps[C] = (value & 0x80) >> 7;
				result = value << 1;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[N] = ((result < 0) ? 1 : 0);

				memory->write(absolute_addr + cpu->x, result);
                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			//BCC
			case(0x90):
				if(cpu->ps[C] == 0)
						cpu->pc += (int8_t) memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BCS
			case(0xb0):
				if(cpu->ps[C] == 1)
						cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BEQ
			case(0xf0):
				if(cpu->ps[Z] == 1)
						cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BIT
			case(0x24): //Zero Page
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[V] = (((value & 0x40) != 0) ? 1 : 0);
				cpu->ps[N] = ((value < 0) ? 1 : 0);

                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			case(0x2c): //Absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += memory->read(++(cpu->pc)) << 8;

				value = memory->read(absolute_addr + cpu->x);

				result = cpu->a & value;

				cpu->ps[Z] = ((result == 0) ? 1 : 0);
				cpu->ps[V] = (((value & 0x40) != 0) ? 1 : 0);
				cpu->ps[N] = ((value < 0) ? 1 : 0);

                memory->was_accessed = true;
				(cpu->pc)++;

				break;
			//BMI
			case(0x30):
				if(cpu->ps[N] == 1)
						cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BNE
			case(0xd0):
				if(cpu->ps[Z] == 0)
						cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BPL
			case(0x10):
				if(cpu->ps[N] == 0)
						cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BRK
			case(0x00):
				if(is_IRQ_enabled){
					cpu->ps[B] = 1;

					value = 0;

					value += ((cpu->ps[N]) ? 1 : 0) << N;
					value += ((cpu->ps[V]) ? 1 : 0) << V;
					value += 1 << (B-1);
					value += ((cpu->ps[B]) ? 1 : 0) << B;
					value += ((cpu->ps[D]) ? 1 : 0) << D;
					value += ((cpu->ps[I]) ? 1 : 0) << I;
					value += ((cpu->ps[Z]) ? 1 : 0) << Z;
					value += ((cpu->ps[C]) ? 1 : 0) << C;

					memory->write((cpu->sp)--, value);

					absolute_addr = memory->IRQ_ADDR;

                    cpu->pc = absolute_addr;
                }
                else {
                    exit_emulation = true;
                    return;
                }
			//BVC
			case(0x50):
                if(cpu->ps[V] == 0)
                    cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//BVS
			case(0x70):
                if(cpu->ps[V] == 1)
                    cpu->pc += (int8_t)  memory->read(++(cpu->pc)) + 1;
				else
					cpu->pc += 2;
				break;
			//CLC
			case(0x18):
				cpu->ps[C] = 0;

				(cpu->pc)++;
				break;
			///
			/// CMP
			///
			case(0xC9): //immediate
			    immediate = memory->read(++(cpu->pc));

				setFlagsCMP(immediate, cpu->a, cpu);

			    (cpu->pc)++;

			    break;
			case(0xC5): //zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				setFlagsCMP(value, cpu->a, cpu);

				(cpu->pc)++;
                memory->was_accessed = true;
			    break;
			case(0xD5): //zero page + x
			    zero_pg_addr= memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr+ cpu->x);

				setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xCD): //absolute
			    absolute_addr= memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr);

			    setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;

			case(0xDD): //absolute + x
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
				value = memory->read(absolute_addr + cpu->x);

				setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xD9): //absolute + y
			    absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc))) << 8;
				value = memory->read(absolute_addr + cpu->y);

			    setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;

			case(0xC1): // (indirect, x)
			    zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr + cpu->x);
				absolute_addr += memory->read(zero_pg_addr + cpu->x + 1) << 8;
				value = memory->read(absolute_addr);

				setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xD1): // (indirect), y
			    zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;
				value = memory->read(absolute_addr + cpu->y);

				setFlagsCMP(value, cpu->a, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
				break;
		    ///
			/// CPX
			///
			case(0xE0): //immediate
			    immediate = memory->read(++(cpu->pc));

				setFlagsCMP(immediate, cpu->x, cpu);

			    (cpu->pc)++;

			    break;
			case(0xE4): //zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				setFlagsCMP(value, cpu->x, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
			    break;
			case(0xEC): //absolute
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr);

			    setFlagsCMP(value, cpu->x, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
		    ///
			/// CPY
			///
			case(0xC0): //immediate
			    immediate = memory->read(++(cpu->pc));

				setFlagsCMP(immediate, cpu->y, cpu);

			    (cpu->pc)++;

			    break;
			case(0xC4): //zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				setFlagsCMP(value, cpu->y, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
			    break;
			case(0xCC): //absolute
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr);

			    setFlagsCMP(value, cpu->y, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;

			///
            /// DEC
			///
			case(0xC6): // zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
                value = memory->read(zero_pg_addr) - 1;

                memory->write(zero_pg_addr, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
			    break;
			case(0xD6): // zero page + x
			    zero_pg_addr = memory->read(++(cpu->pc));
                value = memory->read(zero_pg_addr + cpu->x) - 1;

                memory->write(zero_pg_addr + cpu->x, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xCE): // absolute
			    absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc))) << 8;
                value = memory->read(absolute_addr) - 1;

                memory->write(absolute_addr, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xDE): // absolute + x
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr + cpu->x) - 1;

                memory->write(absolute_addr + cpu->x, value);

			    setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			///
			/// DEX
            ///
			case(0xCA):
				(cpu->x)--;

				setFlagsDEC(cpu->x, cpu);

				(cpu->pc)++;
			    break;
			///
            /// DEY
			///
            case(0x88):
               (cpu->y)--;

               setFlagsDEC(cpu->y, cpu);

               (cpu->pc)++;
               break;
            ///
            /// EOR
			///
			case(0x49): //immediate
			    immediate = memory->read(++(cpu->pc));

				cpu->a = cpu->a ^ immediate;

				setFlagsEOR(cpu->a, cpu);

			    (cpu->pc)++;

			    break;
			case(0x45): //zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);

				cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
			    break;
			case(0x55): //zero page + x
			    zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr + cpu->x);

				cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0x4D): //absolute
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr);

			    cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;

			case(0x5D): //absolute + x
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
				value = memory->read(absolute_addr + cpu->x);

				cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0x59): //absolute + y
			    absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc))) << 8;
				value = memory->read(absolute_addr + cpu->y);

			    cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;

			case(0x41): // (indirect, x)
			    zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr + cpu->x);
				absolute_addr += memory->read(zero_pg_addr + cpu->x + 1) << 8;
				value = memory->read(absolute_addr);

				cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0x51): // (indirect), y
			    zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += memory->read(zero_pg_addr + 1) << 8;
				value = memory->read(absolute_addr + cpu->y);

				cpu->a = cpu->a ^ value;

				setFlagsEOR(cpu->a, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			///
            /// INC
			///
			case(0xE6): // zero page
			    zero_pg_addr = memory->read(++(cpu->pc));
                value = memory->read(zero_pg_addr) + 1;

                memory->write(zero_pg_addr, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
				(cpu->pc)++;
			    break;
			case(0xF6): // zero page + x
			    zero_pg_addr = memory->read(++(cpu->pc));
                value = memory->read(zero_pg_addr + cpu->x) + 1;

                memory->write(zero_pg_addr + cpu->x, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
				break;
			case(0xEE): // absolute
			    absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc))) << 8;
                value = memory->read(absolute_addr) + 1;

                memory->write(absolute_addr, value);

				setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			case(0xFE): // absolute + x
			    absolute_addr = memory->read(++(cpu->pc));
			    absolute_addr += (memory->read(++(cpu->pc))) << 8;
			    value = memory->read(absolute_addr + cpu->x) + 1;

                memory->write(absolute_addr + cpu->x, value);

			    setFlagsDEC(value, cpu);

                memory->was_accessed = true;
			    (cpu->pc)++;
			    break;
			///
			/// INX
            ///
			case(0xE8):
				(cpu->x)++;

				setFlagsDEC(cpu->x, cpu);

				(cpu->pc)++;
			    break;
			///
            /// INY
			///
            case(0xC8):
                (cpu->y)++;

                setFlagsDEC(cpu->y, cpu);

                (cpu->pc)++;
                break;
			///
			/// JMP
			///
            case(0x4C): // absolute
			    absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc))) << 8;
                memory->was_accessed = false;
				cpu->pc = absolute_addr;
                break;
            case(0x6C): // indirect
                absolute_addr = memory->read(++(cpu->pc));
                absolute_addr += (memory->read(++(cpu->pc))) << 8;
                memory->was_accessed = false;
                cpu->pc = memory->read(absolute_addr);
                cpu->pc += memory->read(absolute_addr + 1) << 8;

			    break;

			//
			//JSR
			//
			case(32): //20
				absolute_addr = (cpu->pc) + 3;
				memory->write(0x0100 + (cpu->sp)--, absolute_addr >> 8);
				memory->write(0x0100 + (cpu->sp)--, absolute_addr & 0xFF);
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read(((cpu->pc)+2)) << 8);
				cpu->pc = absolute_addr;
				// cpu->pc += memory->RESET_ADDR;
				memory->was_accessed = false;
				break;
			//
			//LDA
			//
			case(169): //a9 - immediate
				cpu->a = memory->read((cpu->pc)+1);

				if(cpu->a == 0x00)	cpu->ps[Z] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

				cpu->pc += 2;
				break;
			case(165): //a5 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->a = memory->read(zero_pg_addr);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(181): //b5 - zeropage,x
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->a = memory->read(zero_pg_addr+cpu->x);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(173): //ad - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->a = memory->read(absolute_addr);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(189): //bd - absolute, x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->a = memory->read(absolute_addr+cpu->x);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(185): //b9 - absolute, y
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->a = memory->read(absolute_addr+cpu->y);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(161): //a1 - (indirect, x)
				immediate = memory->read((cpu->pc)+1);
				absolute_addr = memory->read((uint8_t) immediate+cpu->x);
				absolute_addr += memory->read(immediate+cpu->x+1) << 8;
				cpu->a = memory->read(absolute_addr);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(177): //b1 - (indrect), y
				immediate = memory->read((cpu->pc)+1);
				absolute_addr = memory->read((uint8_t)immediate);
				absolute_addr += memory->read(immediate+1) << 8;
				cpu->a = memory->read(absolute_addr+cpu->y);

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			//
			//LDX
			//
			case(162): //a2 - immediate
				cpu->x = memory->read((cpu->pc)+1);

				if(cpu->x == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

				cpu->pc += 2;
				break;
			case(166): //a6 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->x = memory->read(zero_pg_addr);

				if(cpu->x == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(182): //b6 - zeropage,y
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->x = memory->read(zero_pg_addr+cpu->y);

				if(cpu->x == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(174): //ae - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->x = memory->read(absolute_addr);

				if(cpu->x == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(190): //be - absolute, y
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->x = memory->read(absolute_addr+cpu->y);

				if(cpu->x == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			//
			//LDY
			//
			case(160): //a0 - immediate
				cpu->y = memory->read((cpu->pc)+1);

				if(cpu->y == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

				cpu->pc += 2;
				break;
			case(164): //a4 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->y = memory->read(zero_pg_addr);

				if(cpu->y == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(180): //b4 - zeropage, x
				zero_pg_addr = memory->read((cpu->pc)+1);
				cpu->y = memory->read(zero_pg_addr+cpu->x);

				if(cpu->y == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(172): //ac - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->y = memory->read(absolute_addr);

				if(cpu->y == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(188): //bc - absolute, x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				cpu->y = memory->read(absolute_addr+cpu->x);

				if(cpu->y == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->y & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			//
			//LSR
			//
			case(74): //4a - accumulator
				cpu->ps[0] = 0;					//always positive
				if((cpu->a & 0x01) == 0x01)		//check carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				cpu->a = cpu->a >> 1;
				cpu->a = cpu->a & 0x7f; 		//mask and 0b01111111

				if(cpu->a == 0x00)			 	//check zero
					cpu->ps[6] = 1;
				else
					cpu->ps[6] = 0;

				cpu->pc += 1;
				break;
			case(70): //46 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				aux = memory->read(zero_pg_addr);

				cpu->ps[0] = 0;					//always positive
				if((aux & 0x01) == 0x1)			//check carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				aux = aux >> 1;
				aux = aux & 0x7f; 		//mask and 0b01111111

				if(aux == 0x00)			 	//check zero
					cpu->ps[6] = 1;
				else
					cpu->ps[6] = 0;

				memory->write(zero_pg_addr, aux);
                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(86): //56 - zeropage, x
				zero_pg_addr = memory->read((cpu->pc)+1);
				aux = memory->read(zero_pg_addr+cpu->x);

				cpu->ps[0] = 0;					//always positive
				if((aux & 0x01) == 0x1)		//check carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				aux = aux >> 1;
				aux = aux & 0x7f; 		//mask and 0b01111111

				if(aux == 0x00)			 	//check zero
					cpu->ps[6] = 1;
				else
					cpu->ps[6] = 0;

				memory->write(zero_pg_addr + cpu->x, aux);
                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(78): //4e - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) <<8);
				aux = memory->read(absolute_addr);

				cpu->ps[0] = 0;					//always positive
				if((aux & 0x01) == 0x1)		//check carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				aux = aux >> 1;
				aux = aux & 0x7f; 		//mask and 0b01111111

				if(aux == 0x00)			 	//check zero
					cpu->ps[6] = 1;
				else
					cpu->ps[6] = 0;

				memory->write(absolute_addr, aux);
                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(94): //5e - absolute,x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) <<8);
				aux = memory->read(absolute_addr+(cpu->x & 0xFF));

				cpu->ps[0] = 0;					//always positive
				if((aux & 0x01) == 0x1)		//check carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				aux = aux >> 1;
				aux = aux & 0x7f; 		//mask and 0b01111111

				if(aux == 0x00)			 	//check zero
					cpu->ps[6] = 1;
				else
					cpu->ps[6] = 0;
				memory->write(absolute_addr+(cpu->x & 0xFF), aux);
                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			//
			//NOP
			//
			case(234): //ea - implied
				cpu->pc += 1;
				break;
			//
			//ORA
			//
			case(9): //09 - immediate
				immediate = memory->read((cpu->pc)+1);
				cpu->a = cpu->a | immediate;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

				cpu->pc += 2;
				break;
			case(5): //05 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				aux = memory->read(zero_pg_addr);
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(21): //15 - zeropage,x
				zero_pg_addr = memory->read((cpu->pc)+1);
				aux = memory->read(zero_pg_addr+cpu->x);
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(13): //0d - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				aux = memory->read(absolute_addr);
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(29): //1d - absolute,x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				aux = memory->read(absolute_addr+(cpu->x & 0xFF));
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(25): //19 - absolute,y
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += (memory->read((cpu->pc)+2) << 8);
				aux = memory->read(absolute_addr+(cpu->y & 0xFF));
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 3;
				break;
			case(1): //01 - (indirect,x)
				immediate = memory->read((cpu->pc)+1);
				absolute_addr = memory->read(immediate+cpu->x);
				absolute_addr += memory->read(immediate+cpu->x+1) << 8;
				aux = memory->read(absolute_addr);
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			case(17): //11 - (indirect), y
				immediate = memory->read((cpu->pc)+1);
				absolute_addr = memory->read(immediate);
				absolute_addr += memory->read(immediate+1) << 8;
				aux = memory->read(absolute_addr+(cpu->y & 0xFF));
				cpu->a = cpu->a | aux;

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 2;
				break;
			//
			//PHA
			//
			case(72): //48 - implied
				memory->write(0x0100 + (cpu->sp-- & 0xFF), cpu->a);
				cpu->pc += 1;
				break;
			//
			//PHP
			//
			case(8): //08 - implied
				cpu->ps[2] = 1;
				cpu->ps[3] = 1;
				aux = cpu->ps[0]*pow(2,7);
				for(int j = 1; j < 8; j++)
					aux += cpu->ps[j]*pow(2,7-j);
				memory->write(0x0100 + (cpu->sp-- & 0xFF), aux);
				cpu->pc += 1;
				break;
			//
			//PLA
			//
			case(104): //68 - implied
				cpu->a = memory->read((0x0100 + (++cpu->sp & 0xFF)));

				if(cpu->a == 0x00)	cpu->ps[6] = 1;
				else cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
				else cpu->ps[0] = 0;

                memory->was_accessed = true;
				cpu->pc += 1;
				break;
			//
			//PLP
			//
			case(40): //28 - implied
				aux = memory->read((0x0100 + (++cpu->sp & 0xFF)));
				for(int i = 0; i < 8; i++) {
					if(i != 2 && i != 3){
						if((aux & 0x80) == 0x80)
							cpu->ps[i] = 1;
						else
							cpu->ps[i] = 0;
					}
					aux = aux << 1;
				}
                memory->was_accessed = true;
				cpu->pc += 1;
				break;
			//
			//ROL
			//
			case(42): //2a - accumulator
				aux = cpu->a;
				cpu->a = (cpu->a << 1);

				if(cpu->ps[7] == 1)				//if previous carry was 1
					cpu->a = (cpu->a | 0x01);

				if(cpu->a == 0)
					cpu->ps[6] = 1;					//verify zero
				else
					cpu->ps[6] = 0;

				if((cpu->a & 0x80) == 0x80)
					cpu->ps[0] = 1;     //verify neg
				else
					cpu->ps[0] = 0;

				if((aux & 0x80) == 0x80)   //update carry
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				cpu->pc += 1;
				break;
			case(38): //26 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				value = memory->read(zero_pg_addr);
				aux = value;

				value = (value << 1);

				if(cpu->ps[7] == 1)
					value = (value | 0x01);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x80) == 0x80)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(zero_pg_addr, value);
				cpu->pc += 2;
				break;
			case(54): //36 - zeropage,x
				zero_pg_addr = memory->read((cpu->pc)+1);
				value = memory->read(zero_pg_addr + cpu->x);
				aux = value;

				value = (value << 1);

				if(cpu->ps[7] == 1)
					value = (value | 0x01);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x80) == 0x80)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(zero_pg_addr+cpu->x, value);
				cpu->pc += 2;
				break;
			case(46): //2e - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += memory->read((cpu->pc)+2) << 8;
				value = memory->read(absolute_addr);
				aux = value;

				value = (value << 1);

				if(cpu->ps[7] == 1)
					value = (value | 0x01);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x80) == 0x80)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(absolute_addr, value);
				cpu->pc += 3;
				break;
			case(62): //3e - absolute,x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += memory->read((cpu->pc)+2) << 8;
				value = memory->read(absolute_addr+(cpu->x & 0xFF));
				aux = value;

				value = (value << 1);

				if(cpu->ps[7] == 1)
					value = (value | 0x01);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x80) == 0x80)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(absolute_addr+(cpu->x & 0xFF), value);
				cpu->pc += 3;
				break;
			//
			//ROR
			case(106): //6a - accumulator
				aux = cpu->a;
				cpu->a = (cpu->a >> 1);
				cpu->a = cpu->a & 0x7f; 		//mask and 0b01111111

				if(cpu->ps[C] == 1) {
					cpu->a = (cpu->a | 0x80);
					cpu->ps[C] = aux & 0x01;					//is negative
				}

				if(cpu->a == 0)
					cpu->ps[Z] = 1;					//verify zero
				else
					cpu->ps[Z] = 0;

				if((cpu->a & 0x80) == 0x80)
					cpu->ps[N] = 1;     //verify neg
				else
					cpu->ps[N] = 0;

				if((aux & 0x01) == 0x01)   //update carry
					cpu->ps[C] = 1;
				else
					cpu->ps[C] = 0;

				cpu->pc += 1;
				break;
			case(102): //66 - zeropage
				zero_pg_addr = memory->read((cpu->pc)+1);
				value = memory->read(zero_pg_addr);
				aux = value;

				value = (value >> 1);
				value = value & 0x7f; 		//mask and 0b01111111

				if(cpu->ps[C] == 1)
					value = (value | 0x80);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x01) == 0x01)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(zero_pg_addr, value);
				cpu->pc += 2;
				break;
			case(118): //76 - zeropage,x
				zero_pg_addr = memory->read((cpu->pc)+1) + cpu->x;
				value = memory->read(zero_pg_addr);
				aux = value;

				value = (value >> 1);
				value = value & 0x7f; 		//mask and 0b01111111

				if(cpu->ps[7] == 1)
					value = (value | 0x80);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x01) == 0x01)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(zero_pg_addr, value);
				cpu->pc += 2;
				break;
			case(110): //6e - absolute
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += memory->read((cpu->pc)+2) << 8;
				value = memory->read(absolute_addr);
				aux = value;

				value = (value >> 1);
				value = value & 0x7f; 		//mask and 0b01111111

				if(cpu->ps[7] == 1)
					value = (value | 0x80);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x01) == 0x01)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(absolute_addr, value);
				cpu->pc += 3;
				break;
			case(126): //7e - absolute,x
				absolute_addr = memory->read((cpu->pc)+1);
				absolute_addr += memory->read((cpu->pc)+2) << 8;
				absolute_addr += (cpu->x & 0xFF);
				value = memory->read(absolute_addr);
				aux = value;

				value = (value >> 1);
				value = value & 0x7f; 		//mask and 0b01111111

				if(cpu->ps[7] == 1)
					value = (value | 0x80);

				if (value == 0)
					cpu->ps[Z] = 1;
				else cpu->ps[Z] = 0;

				if((value & 0x80) == 0x80)
					cpu->ps[0] = 1;
				else
					cpu->ps[0] = 0;

				if((aux & 0x01) == 0x01)
					cpu->ps[7] = 1;
				else
					cpu->ps[7] = 0;

				memory->write(absolute_addr, value);
				cpu->pc += 3;
				break;
			//
			//RTI
			//
			case(64): //40 - implied
				aux = memory->read(0x0100 + (++cpu->sp));
				for(int i = 0; i < 8; i++) {
					if((aux & 0x01) == 0x01)
						cpu->ps[i] = 1;
					else
						cpu->ps[i] = 0;
					aux = aux >> 1;
					aux = aux & 0x7f; 		//mask and 0b01111111
				}

                memory->was_accessed = false;
				cpu->pc = memory->read(0x0100 + (++cpu->sp) & 0xFF);
				cpu->pc += memory->read(0x0100 + (++cpu->sp)) << 8 ;

				break;
			//RTS
			case(96): 	//60 -- implied
				absolute_addr = memory->read(0x0100 + (++cpu->sp));
				absolute_addr += memory->read(0x0100 + (++cpu->sp)) << 8;
				cpu->pc = absolute_addr;
				// cpu->pc += memory->RESET_ADDR;
                memory->was_accessed = false;
				break;
			//SBC
			case(233): 	//e9 -- immediate
				immediate = memory->read(++(cpu->pc));
				sbc(cpu, immediate);
				(cpu->pc)++;
				break;
			case(229): 	//e5 -- zero page
				zero_pg_addr= memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
 			case(245): 	//f5 -- zero page, x
				zero_pg_addr = memory->read(++(cpu->pc));
				value = memory->read(zero_pg_addr + cpu->x);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			case(237): 	//ed -- absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				value = memory->read(absolute_addr);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			case(253): 	//fd -- absolute, x
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				value = memory->read(absolute_addr+cpu->x);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			case(249): 	//f9 -- absolute, y
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				value = memory->read(absolute_addr+cpu->y);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			case(225): 	//e1 -- (indirect, x)
				zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr+cpu->x);
                absolute_addr += memory->read(zero_pg_addr + cpu->x + 1) << 8;
				value = memory->read(absolute_addr);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			case(241): 	//f1 -- (indirect), y
				zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr);
				absolute_addr += (memory->read(zero_pg_addr + 1) << 8);
				value = memory->read(absolute_addr+cpu->y);
				sbc(cpu, value);
                memory->was_accessed = true;
				(cpu->pc)++;
				break;
			//SEC
			case(56):		//38 -- implied
				cpu->ps[C] = 1;
				(cpu->pc)++;
				break;
			// SED
			case(248):	//f8 -- implied
				cpu->ps[D] = 1;
				(cpu->pc)++;
				break;
			// SEI
			case(120):	//78 -- implied
				cpu->ps[I] = 1;
				(cpu->pc)++;
				break;
			// STA
			case(133): 	//85 -- zero page
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr, cpu->a);
				(cpu->pc)++;
				break;
			case(149): 	//95 -- zero page, x
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr+cpu->x, cpu->a);
				(cpu->pc)++;
				break;
			case(141): 	//8d -- absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				memory->write(absolute_addr, cpu->a);
				(cpu->pc)++;
				break;
			case(157): 	//9d -- absolute, x
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				memory->write(absolute_addr+cpu->x, cpu->a);
				(cpu->pc)++;
				break;
			case(153): 	//99 -- absolute, y
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				memory->write(absolute_addr+cpu->y, cpu->a);
				(cpu->pc)++;
				break;
			case(129): 	//81 -- (indirect, x)
				zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = memory->read(zero_pg_addr+cpu->x);
				absolute_addr += memory->read(zero_pg_addr+cpu->x+1) << 8;
				memory->write(absolute_addr, cpu->a);
				(cpu->pc)++;
				break;
			case(145): 	//91 -- (indirect), y
				zero_pg_addr = memory->read(++(cpu->pc));
				absolute_addr = (memory->read(zero_pg_addr));
				absolute_addr += (memory->read(zero_pg_addr + 1) << 8);
				memory->write(absolute_addr+cpu->y, cpu->a);
				(cpu->pc)++;
				break;
			// STX
			case(134): 	//86 -- zero page
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr, cpu->x);
				(cpu->pc)++;
				break;
			case(150): 	//96 -- zero page, y
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr+cpu->y, cpu->x);
				(cpu->pc)++;
				break;
			case(142): 	//8e -- absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				memory->write(absolute_addr, cpu->x);
				(cpu->pc)++;
				break;
			// STY
			case(132): 	//84 -- zero page
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr, cpu->y);
				(cpu->pc)++;
				break;
			case(148): 	//94 -- zero page, x
				zero_pg_addr = memory->read(++(cpu->pc));
				memory->write(zero_pg_addr+cpu->x, cpu->y);
				(cpu->pc)++;
				break;
			case(140): 	//8c -- absolute
				absolute_addr = memory->read(++(cpu->pc));
				absolute_addr += (memory->read(++(cpu->pc)) << 8);
				memory->write(absolute_addr, cpu->y);
				(cpu->pc)++;
				break;
			// TAX
			case(170): 	//aa -- implied
				cpu->x = cpu->a;
				if (cpu->x == 0) cpu->ps[Z] = 1;
                else cpu->ps[Z] = 0;
				if (cpu->x < 0) cpu->ps[N] = 1;
                else cpu->ps[N] = 0;
				(cpu->pc)++;
				break;
			// TAY
			case(168): 	//a8 -- implied
				cpu->y = cpu->a;
				if (cpu->y == 0) cpu->ps[Z] = 1;
                else cpu->ps[Z] = 0;
				if (cpu->y < 0) cpu->ps[N] = 1;
                else cpu->ps[N] = 0;
				(cpu->pc)++;
				break;
			// TSX
			case(186): 	//ba -- implied
				cpu->x = cpu->sp;
				if (cpu->x == 0) cpu->ps[Z] = 1;
                else cpu->ps[Z] = 0;
				if (cpu->x < 0) cpu->ps[N] = 1;
                else cpu->ps[N] = 0;
				(cpu->pc)++;
				break;
			// TXA
			case(138): 	//8a -- implied
				cpu->a = cpu->x;
				if (cpu->a == 0) cpu->ps[Z] = 1;
                else cpu->ps[Z] = 0;
				if (cpu->a < 0) cpu->ps[N] = 1;
                else cpu->ps[N] = 0;
				(cpu->pc)++;
				break;
			// TXS
			case(154): 	//9a -- implied
				cpu->sp = cpu->x;
				(cpu->pc)++;
				break;
			// TYA
			case(152): 	//98 -- implied
				cpu->a = cpu->y;
				if (cpu->a == 0) cpu->ps[Z] = 1;
                else cpu->ps[Z] = 0;
				if (cpu->a < 0) cpu->ps[N] = 1;
                else cpu->ps[N] = 0;
				(cpu->pc)++;
				break;

			case(0x58): // CLI
			    cpu->ps[I] = 0;
				(cpu->pc)++;
			    break;
			case(0xB8): // CLV
			    cpu->ps[V] = 0;
				(cpu->pc)++;
				break;
			case(0xD8): // CLD
			    cpu->ps[D] = 0;
				(cpu->pc)++;
				break;

			default:
				printf("erro\n");
				(cpu->pc)++;
                break;
        }

        // A memoria guarda a cada ciclo se foi escrita nela
        // e qual foi o ultimo endereco que foi modificado
        if (memory->was_accessed)
            logls(memory, cpu);
        else
			log(cpu);

        memory->was_accessed = false;
	}
}
*/
