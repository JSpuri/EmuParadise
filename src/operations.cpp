#include "headers/operations.hpp"

#include <ios>
#include <fstream>
#include <iostream>
#include <math.h>

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

    if(((num > 0) && (cpu->a > 0) && (result <= 0)) || ((num < 0) && (cpu->a < 0) && (result >= 0))) {
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
    //printf("ADC %02x + (cpu->A: %02x) = ", num, cpu->a);

    adc_aux(cpu, num);
    //printf("%02x\n", cpu->a);
}

void AND(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

    int8_t result = cpu->a & value;
    //printf("AND %02x & (cpu->A: %02x) = %02x", value, cpu->a, result);

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[N] = ((result < 0) ? 1 : 0);

    cpu->a = result;
}

void ASL(int mode, CPU *cpu) {

    int8_t result;

    if(mode == M_ACCUMULATOR){

        cpu->ps[C] = (cpu->a & 0x80) >> 7; 
        result = cpu->a << 1;
        //printf("ASL (cpu->A: %02x) = %02x\n", cpu->a, result);

        cpu->a = result;
    }
    else{

        int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
        cpu->ps[C] = (value & 0x80) >> 7;
        result = value << 1;

        uint16_t target_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
        //printf("ASL (M[%02x]: %02x) = %02x\n", target_addr, value, result);
        cpu->WriteTo(target_addr, result);
    }

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[N] = ((result < 0) ? 1 : 0);
}

void BCC(int mode, CPU *cpu) {
    //printf("BCC (pc before): %02x\n", cpu->pc);
    if(cpu->ps[C] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BCC (pc after): %02x\n", cpu->pc);
}

void BCS(int mode, CPU *cpu) {
    //printf("BCS (pc before): %02x\n", cpu->pc);
    if(cpu->ps[C] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BCS (pc after): %02x\n", cpu->pc);
}

void BEQ(int mode, CPU *cpu) {
    //printf("BEQ (pc before): %02x\n", cpu->pc);
    if(cpu->ps[Z] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BEQ (pc after): %02x\n", cpu->pc);
}

void BIT(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    int8_t result = cpu->a & value;
    //printf("BIT %02x (cpu->A: %02x) => %02x\n", value, cpu->a, result);

    cpu->ps[Z] = ((result == 0) ? 1 : 0);
    cpu->ps[V] = (((value & 0x40) != 0) ? 1 : 0);
    cpu->ps[N] = ((value < 0) ? 1 : 0);
}

void BMI(int mode, CPU *cpu) {
    //printf("BMI (pc before): %02x\n", cpu->pc);
    if(cpu->ps[N] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BMI (pc after): %02x\n", cpu->pc);
}

void BNE(int mode, CPU *cpu) {
    //printf("BNE (pc before): %02x\n", cpu->pc);
    if(cpu->ps[Z] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BNE (pc after): %02x\n", cpu->pc);
}

void BPL(int mode, CPU *cpu) {
    //printf("BPL (pc before): %02x\n", cpu->pc);
    if(cpu->ps[N] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BPL (pc after): %02x\n", cpu->pc);
}

void BRK(int mode, CPU *cpu) {
}

void BVC(int mode, CPU *cpu) {
    //printf("BVC (pc before): %02x\n", cpu->pc);
    if(cpu->ps[V] == 0)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BVC (pc after): %02x\n", cpu->pc);
}

void BVS(int mode, CPU *cpu) {
    //printf("BVS (pc before): %02x\n", cpu->pc);
    if(cpu->ps[V] == 1)
        cpu->pc += (int8_t) cpu->ResolveOPArgWord(mode, cpu->pc + 1) + 2;
    //printf("BVS (pc after): %02x\n", cpu->pc);
}

void CLC(int mode, CPU *cpu) {
    //printf("CLC\n");
    cpu->ps[C] = 0;
}

void CLD(int mode, CPU *cpu) {
    //printf("CLD\n");
    cpu->ps[D] = 0;
}

void CLI(int mode, CPU *cpu) {
    //printf("CLI\n");
    cpu->ps[I] = 0;
}

void CLV(int mode, CPU *cpu) {
    //printf("CLV\n");
    cpu->ps[V] = 0;
}

void CMP(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    //printf("CMP %02x (cpu->A: %02x)\n", value, cpu->a);
    setFlagsCMP(value, cpu->a, cpu);
}

void CPX(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    //printf("CPX %02x (cpu->X: %02x)\n", value, cpu->x);
    setFlagsCMP(value, cpu->x, cpu);
}

void CPY(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    //printf("CPY %02x (cpu->Y: %02x)\n", value, cpu->y);
    setFlagsCMP(value, cpu->y, cpu);
}

void DEC(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    value -= 1;

    setFlagsDEC(value, cpu);

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("DEC (M[%02x]: %02x)\n", addr, value + 1);
    cpu->WriteTo(addr, value);

}

void DEX(int mode, CPU *cpu) {

    //printf("DEX (cpu->X: %02x)\n", cpu->x);
    cpu->x -= 1;
    setFlagsDEC(cpu->x, cpu);
}

void DEY(int mode, CPU *cpu) {

    //printf("DEY (cpu->Y: %02x)\n", cpu->y);
    cpu->y -= 1;
    setFlagsDEC(cpu->y, cpu);
}

void EOR(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    //printf("EOR %02x ^ (cpu->A: %02x) = ", value, cpu->a);
    cpu->a = cpu->a ^ value;
    //printf("%02x\n", cpu->a);

    setFlagsEOR(cpu->a, cpu);
}

void INC(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    value += 1;

    setFlagsDEC(value, cpu);

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("INC (M[%02x]: %02x)\n", addr, value + 1);
    cpu->WriteTo(addr, value);

}

void INX(int mode, CPU *cpu) {

    //printf("INX (cpu->X: %02x)\n", cpu->x);
    cpu->x += 1;
    setFlagsDEC(cpu->x, cpu);
}

void INY(int mode, CPU *cpu) {

    //printf("DEY (cpu->Y: %02x)\n", cpu->y);
    cpu->y += 1;
    setFlagsDEC(cpu->y, cpu);
}

void JMP(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("JMP %02x\n", addr);
    cpu->pc = addr;
}

void JSR(int mode, CPU *cpu) {
    
    uint16_t absolute_addr = (cpu->pc) + 3;
    
    cpu->WriteTo(0x0100 + (cpu->sp)--, absolute_addr >> 8);
    cpu->WriteTo(0x0100 + (cpu->sp)--, absolute_addr & 0xFF);

    absolute_addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("JSR %02x\n", absolute_addr);
    cpu->pc = absolute_addr;

}

void LDA(int mode, CPU *cpu) {

    cpu->a = cpu->ResolveOPArgWord(mode, cpu->pc +1);
    //printf("LDA %02x\n", cpu->a);

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void LDX(int mode, CPU *cpu) {

    cpu->x = cpu->ResolveOPArgWord(mode, cpu->pc +1);
    //printf("LDX %02x\n", cpu->x);

    if(cpu->x == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->x & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void LDY(int mode, CPU *cpu) {

    cpu->y = cpu->ResolveOPArgWord(mode, cpu->pc +1);
    //printf("LDY %02x\n", cpu->y);

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

        //printf("LSR (cpu->A: %02x) = ", cpu->a);
        cpu->a = cpu->a >> 1;
        cpu->a = cpu->a & 0x7f; 		//mask and 0b01111111
        //printf("%02x\n", cpu->a);

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
        //printf("LSR (M[%02x]: %02x)\n", target_addr, value);
        cpu->WriteTo(target_addr, value);
    }
}

void NOP(int mode, CPU *cpu) {
}

void ORA(int mode, CPU *cpu) {

    int8_t value = cpu->ResolveOPArgWord(mode, cpu->pc + 1);
    //printf("ORA %02x | (cpu->A: %02x) = ", value, cpu->a);
    cpu->a = cpu->a | value;
    //printf("%02x\n", cpu->a);

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void PHA(int mode, CPU *cpu) {
    //printf("PHA\n");
    cpu->WriteTo(0x0100 + (cpu->sp--), cpu->a);
}

void PHP(int mode, CPU *cpu) {

    cpu->ps[2] = 1;
    cpu->ps[3] = 1;
    int8_t aux = cpu->ps[0]*pow(2,7);

    for(int j = 1; j < 8; j++)
        aux += cpu->ps[j]*pow(2,7-j);
    
    //printf("PHP (cpu->ps: %02x\n", aux);

    cpu->WriteTo(0x0100 + (cpu->sp--), aux);

}

void PLA(int mode, CPU *cpu) {

    //printf("PLA (cpu->A: %02x => ", cpu->a);
    cpu->a = cpu->ReadFrom((0x0100 + (++cpu->sp)));
    //printf("%02x\n", cpu->a);

    if(cpu->a == 0x00)	cpu->ps[6] = 1;
    else cpu->ps[6] = 0;

    if((cpu->a & 0x80) == 0x80)	cpu->ps[0] = 1;
    else cpu->ps[0] = 0;

}

void PLP(int mode, CPU *cpu) {

    //printf("PLP\n");
    int8_t aux = cpu->ReadFrom((0x0100 + (++cpu->sp)));

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

        //printf("ROL (cpu->A: %02x) => %02x\n", aux, cpu->a);
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
        //printf("ROL (M[%02x]: %02x)\n", target_addr, value);
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

        //printf("ROR (cpu->A: %02x) => %02x\n", aux, cpu->a);
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
        //printf("LSR (M[%02x]: %02x) => %02x\n", target_addr, aux, value);
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

    //printf("RTI (cpu->pc : %02x) => ", cpu->pc);
    cpu->pc = cpu->ReadFrom((0x0100 + (++cpu->sp)));
    cpu->pc += cpu->ReadFrom(0x0100 + (++cpu->sp)) << 8 ;
    //printf("%02x\n", cpu->pc);
}

void RTS(int mode, CPU *cpu) {

    uint16_t absolute_addr = cpu->ReadFrom(0x0100 + (++cpu->sp));
    absolute_addr += cpu->ReadFrom(0x0100 + (++cpu->sp)) << 8;

    //printf("RTS (cpu->pc : %02x) => %02x\n", cpu->pc, absolute_addr);
    cpu->pc = absolute_addr;

}

void SBC(int mode, CPU *cpu) { 

    int8_t operand = cpu->ResolveOPArgWord(mode, cpu->pc + 1);

	int8_t op1, op2, result;
	result = cpu->a - operand;
	op1 = cpu->a;
	op2 = operand;

	int8_t num = (operand ^ 0xff);
	adc_aux(cpu, num);
	cpu->a -= 1;

	if((op1 >= 0 && op2 > 0 && result < 0))
		cpu->ps[C] = 1;
	else
		cpu->ps[C] = 0;

    //printf("SBC (cpu->A: %02x) - %02x = %02x\n", op1, op2, cpu->a);
}

void SEC(int mode, CPU *cpu) {
    //printf("SEC\n");
    cpu->ps[C] = 1;
}

void SED(int mode, CPU *cpu) {
    //printf("SED\n");
    cpu->ps[D] = 1;
}

void SEI(int mode, CPU *cpu) {
    //printf("SEI\n");
    cpu->ps[I] = 1;
}

void STA(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("STA (cpu->A: %02x) => (M[%02x])\n", cpu->a, addr);
    cpu->WriteTo(addr, cpu->a);
}

void STX(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("STX (cpu->X: %02x) => (M[%02x])\n", cpu->x, addr);
    cpu->WriteTo(addr, cpu->x);
}

void STY(int mode, CPU *cpu) {

    uint16_t addr = cpu->ResolveOPArgAddr(mode, cpu->pc + 1);
    //printf("STY (cpu->Y: %02x) => (M[%02x])\n", cpu->y, addr);
    cpu->WriteTo(addr, cpu->y);
}

void TAX(int mode, CPU *cpu) {

    //printf("TAX\n");
    cpu->x = cpu->a;
    if (cpu->x == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->x < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TAY(int mode, CPU *cpu) {

    //printf("TAY\n");
    cpu->y = cpu->a;
    if (cpu->y == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->y < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TSX(int mode, CPU *cpu) {

    //printf("TSX\n");
    cpu->x = cpu->sp;
    if (cpu->x == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->x < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TXA(int mode, CPU *cpu) {

    //printf("TXA\n");
    cpu->a = cpu->x;
    if (cpu->a == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->a < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

void TXS(int mode, CPU *cpu) {
    //printf("TXS\n");
    cpu->sp = cpu->x;
}

void TYA(int mode, CPU *cpu) {

    //printf("TYA\n");
    cpu->a = cpu->y;
    if (cpu->a == 0) cpu->ps[Z] = 1;
    else cpu->ps[Z] = 0;
    if (cpu->a < 0) cpu->ps[N] = 1;
    else cpu->ps[N] = 0;

}

