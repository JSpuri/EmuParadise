#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <bits/stdc++.h>

#include "cpu.hpp"
#include "memory.hpp"

void ADC(int mode, CPU *cpu, Memory *memory);
void AND(int mode, CPU *cpu, Memory *memory);
void ASL(int mode, CPU *cpu, Memory *memory);
void BCC(int mode, CPU *cpu, Memory *memory);
void BCS(int mode, CPU *cpu, Memory *memory);
void BEQ(int mode, CPU *cpu, Memory *memory);
void BIT(int mode, CPU *cpu, Memory *memory);
void BMI(int mode, CPU *cpu, Memory *memory);
void BNE(int mode, CPU *cpu, Memory *memory);
void BPL(int mode, CPU *cpu, Memory *memory);
void BRK(int mode, CPU *cpu, Memory *memory);
void BVC(int mode, CPU *cpu, Memory *memory);
void BVS(int mode, CPU *cpu, Memory *memory);
void CLC(int mode, CPU *cpu, Memory *memory);

void CLD(int mode, CPU *cpu, Memory *memory);
void CLI(int mode, CPU *cpu, Memory *memory);
void CLV(int mode, CPU *cpu, Memory *memory);
void CMP(int mode, CPU *cpu, Memory *memory);
void CPX(int mode, CPU *cpu, Memory *memory);
void CPY(int mode, CPU *cpu, Memory *memory);
void DEC(int mode, CPU *cpu, Memory *memory);
void DEX(int mode, CPU *cpu, Memory *memory);
void DEY(int mode, CPU *cpu, Memory *memory);
void EOR(int mode, CPU *cpu, Memory *memory);
void INC(int mode, CPU *cpu, Memory *memory);
void INX(int mode, CPU *cpu, Memory *memory);
void INY(int mode, CPU *cpu, Memory *memory);
void JMP(int mode, CPU *cpu, Memory *memory);

void JSR(int mode, CPU *cpu, Memory *memory);
void LDA(int mode, CPU *cpu, Memory *memory);
void LDX(int mode, CPU *cpu, Memory *memory);
void LDY(int mode, CPU *cpu, Memory *memory);
void LSR(int mode, CPU *cpu, Memory *memory);
void NOP(int mode, CPU *cpu, Memory *memory);
void ORA(int mode, CPU *cpu, Memory *memory);
void PHA(int mode, CPU *cpu, Memory *memory);
void PHP(int mode, CPU *cpu, Memory *memory);
void PLA(int mode, CPU *cpu, Memory *memory);
void PLP(int mode, CPU *cpu, Memory *memory);
void ROL(int mode, CPU *cpu, Memory *memory);
void ROR(int mode, CPU *cpu, Memory *memory);
void RTI(int mode, CPU *cpu, Memory *memory);

void RTS(int mode, CPU *cpu, Memory *memory);
void SBC(int mode, CPU *cpu, Memory *memory);
void SEC(int mode, CPU *cpu, Memory *memory);
void SED(int mode, CPU *cpu, Memory *memory);
void SEI(int mode, CPU *cpu, Memory *memory);
void STA(int mode, CPU *cpu, Memory *memory);
void STX(int mode, CPU *cpu, Memory *memory);
void STY(int mode, CPU *cpu, Memory *memory);
void TAX(int mode, CPU *cpu, Memory *memory);
void TAY(int mode, CPU *cpu, Memory *memory);
void TSX(int mode, CPU *cpu, Memory *memory);
void TXA(int mode, CPU *cpu, Memory *memory);
void TXS(int mode, CPU *cpu, Memory *memory);
void TYA(int mode, CPU *cpu, Memory *memory);

#endif
