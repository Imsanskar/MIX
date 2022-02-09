#pragma once
#include <inttypes.h>
#include "./instruction.h"
#include <vector>

struct Register{
    uint64_t value = 0;
    uint8_t noOfBytes = 2;
    uint8_t sign = 0;
};

enum Comparison{
    LESS,
    EQUAL,
    GREATER  
};

struct MIX{
    Register A = Register{
        0,
        5,
        0
    };
    
    Register X = Register{
        0,
        5,
        0
    };
    Register I[5];
    
    struct Register J = Register{
        0, 2, 0
    };

    Comparison comparison_flag;
    bool overflow_flag;
    uint16_t pc;

    Memory memory[4095];

};



uint64_t create_instruction(Memory is){
    uint64_t result;
    uint64_t temp;
    result = is.AA;
    result = result << (6 * 3);
    // for I
    temp = is.I;
    result = result | (temp << 12);

    // for F
    temp = is.F;
    result = result | (temp << 6);

    // for opcode
    temp = is.op;
    result |= temp;


    return result;
}

inline uint32_t interpret_F(uint8_t F, uint64_t operand){
    uint32_t result = operand;
    uint8_t l = F / 8;
    uint8_t r = F % 8;

    result = operand >> ((5 - r) * 8);
    result = result & (2 << ((l - 1) * 8));

    return result;
}

void interpret(MIX *mp, const std::vector<Memory> instructions){
    const int size = instructions.size();
    while(mp->pc < size){
        Memory is = mp->memory[mp->pc];
        // load instruction interpret
        if(is.op == LDA){
            mp->A.value = interpret_F(is.F, mp->memory[is.AA + mp->I[is.F].value]);
        }
        else if(is.op == LDX){
            mp->A.value = interpret_F(is.F, mp->memory[is.AA + mp->I[is.F].value]);
        }
        else if(is.op == INCA){
            mp->A.value += is.AA;
        }
        else if(is.op == STA){
            mp->memory[is.AA + mp->I[is.F].value] = mp->A.value;
        }
        mp->pc += 1;
    }
}
