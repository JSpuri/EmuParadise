#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "../common/constants.hpp"
#include <cstdlib>
#include <cstdint>
#include <vector>

class Processor {

    public:
        virtual void WriteTo(uint16_t addr, uint8_t value) =0;
        virtual uint8_t ReadFrom(uint16_t addr) =0;

        virtual ~Processor() {};
};

#endif

