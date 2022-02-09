#pragma once
#include <stdint.h>
#include "./tokenizer.h"

// each memory contains 5 bytes and 1 memory cell
// instruction also is in this same pattern so this is used
struct Memory{
    uint8_t F; // instruction modifier
    uint8_t I; // index if 0 no address is added to it else one of the index register value is chosen, value ranges from 1 to 6
    uint32_t AA; // address of the memory
    Opcode op; // actual opcode
    bool sign; // positive if false and negative is true
    Memory(){

    }

    Memory(int64_t instruction){
        // as a simulation 8 bits is used but original MIX is supposed to have 6 bits ina byte which is a pain in the ass
        op = static_cast<Opcode>(instruction & 63);
        F = (instruction >> 8) & 63;
        I = (instruction >> 16) & 63;
        AA = (instruction >> 24) && (2 << 12 - 1);
        sign = instruction >> 40 == 1;

    }

    Memory(uint8_t _F, uint8_t _I, int _AA, Opcode _op){
        F = _F;
        I =_I;
        if(_AA >  0){
            AA = _AA;
        }
        else {
            _AA = - _AA;
            sign = true;;
        }
        op = _op;
    }

    operator uint64_t(){
        uint64_t result;
        uint64_t temp;
        result = AA << (8 * 3);
        // for I
        temp = I;
        result = result | (temp << 16);
    
        // for F
        temp = F;
        result = result | (temp << 8);
    
        // for opcode
        temp = op;
        result |= temp;
    
        result |= sign? ((uint64_t)2 << 40):0; 
    
        return result;
    }
};
