#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <cstdlib>
#include <math.h>

#include "cpu.hpp"

void ADC(int mode, CPU *cpu);
void AND(int mode, CPU *cpu);
void ASL(int mode, CPU *cpu);
void BCC(int mode, CPU *cpu);
void BCS(int mode, CPU *cpu);
void BEQ(int mode, CPU *cpu);
void BIT(int mode, CPU *cpu);
void BMI(int mode, CPU *cpu);
void BNE(int mode, CPU *cpu);
void BPL(int mode, CPU *cpu);
void BRK(int mode, CPU *cpu);
void BVC(int mode, CPU *cpu);
void BVS(int mode, CPU *cpu);
void CLC(int mode, CPU *cpu);

void CLD(int mode, CPU *cpu);
void CLI(int mode, CPU *cpu);
void CLV(int mode, CPU *cpu);
void CMP(int mode, CPU *cpu);
void CPX(int mode, CPU *cpu);
void CPY(int mode, CPU *cpu);
void DEC(int mode, CPU *cpu);
void DEX(int mode, CPU *cpu);
void DEY(int mode, CPU *cpu);
void EOR(int mode, CPU *cpu);
void INC(int mode, CPU *cpu);
void INX(int mode, CPU *cpu);
void INY(int mode, CPU *cpu);
void JMP(int mode, CPU *cpu);

void JSR(int mode, CPU *cpu);
void LDA(int mode, CPU *cpu);
void LDX(int mode, CPU *cpu);
void LDY(int mode, CPU *cpu);
void LSR(int mode, CPU *cpu);
void NOP(int mode, CPU *cpu);
void ORA(int mode, CPU *cpu);
void PHA(int mode, CPU *cpu);
void PHP(int mode, CPU *cpu);
void PLA(int mode, CPU *cpu);
void PLP(int mode, CPU *cpu);
void ROL(int mode, CPU *cpu);
void ROR(int mode, CPU *cpu);
void RTI(int mode, CPU *cpu);

void RTS(int mode, CPU *cpu);
void SBC(int mode, CPU *cpu);
void SEC(int mode, CPU *cpu);
void SED(int mode, CPU *cpu);
void SEI(int mode, CPU *cpu);
void STA(int mode, CPU *cpu);
void STX(int mode, CPU *cpu);
void STY(int mode, CPU *cpu);
void TAX(int mode, CPU *cpu);
void TAY(int mode, CPU *cpu);
void TSX(int mode, CPU *cpu);
void TXA(int mode, CPU *cpu);
void TXS(int mode, CPU *cpu);
void TYA(int mode, CPU *cpu);

#endif
