#pragma once
#include <stdint.h>
#include "./tokenizer.h"

// each instruction contains 5 bytes and 1 memory cell
struct Instruction{
    uint8_t F; // instruction modifier
    uint8_t I; // index if 0 no address is added to it else one of the index register value is chosen, value ranges from 1 to 6
    uint32_t AA; // address of the memory
    Opcode op; // actual opcode


    Instruction(){

    }

    Instruction(uint64_t instruction){
        op = static_cast<Opcode>(instruction & 63);
        F = (instruction >> 6) & 63;
        I = (instruction >> 12) & 63;
        AA = instruction >> 18;

    }

    Instruction(uint8_t _F, uint8_t _I, uint32_t _AA, Opcode _op){
        F = _F;
        I =_I;
        AA = _AA;
        op = _op;
    }

    operator int64_t(){
        uint64_t result;
        uint64_t temp;
        result = AA;
        result = result << (6 * 3);
        // for I
        temp = I;
        result = result | (temp << 12);
    
        // for F
        temp = F;
        result = result | (temp << 6);
    
        // for opcode
        temp = op;
        result |= temp;
    
    
        return result;
    }
};
