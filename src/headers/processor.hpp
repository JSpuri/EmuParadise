#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdlib>
#include <cstdint>
#include <vector>

class Processor {

    public:
        virtual void WriteTo(uint16_t addr, int8_t value);
        virtual uint8_t ReadFrom(uint16_t addr);
};

#endif

