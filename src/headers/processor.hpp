#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdlib>
#include <cstdint>
#include <vector>

class AddressBus;

class Processor {

    public:
        virtual void WriteTo(uint16_t addr, int8_t value) =0;
        virtual uint8_t ReadFrom(uint16_t addr) =0;
        virtual ~Processor() {};
    protected:
        AddressBus *addr_bus;
};

#endif

