#include "memory.hpp"
#include "cpu.hpp"

class Opcodes {
    public:
        Opcodes(Memory *memory, CPU *cpu);
        int ADC(int mode);

    private:
        Memory *memory;
        CPU *cpu;
}