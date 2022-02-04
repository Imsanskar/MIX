#include <inttypes.h>


struct Register{
    uint64_t value;
    const uint8_t noOfBytes;
    uint8_t sign = 0;
};

enum Comparison{
    LESS,
    EQUAL,
    GREATER  
};

struct MIX{
    struct Register A = Register{
        0,
        5,
        0
    };
    
    struct Register X = Register{
        0,
        5,
        0
    };
    struct Register I[5] = {
        Register{
            0, 2, 0
        }
    };
    
    struct Register J = Register{
        0, 2, 0
    };

    Comparison comparison_flag;
    bool overflow_flag;

};

enum Opcode{
    LDA = 8,
};

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
};

uint64_t create_instruction(Instruction is){
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

Instruction create_instruction(uint64_t value){
    Instruction is;
    is.op = static_cast<Opcode>(value & 63);
    is.F = (value >> 6) & 63;
    is.I = (value >> 12) & 63;
    is.AA = value >> 18;

    return is;
}


uint64_t memory[4095];

void interpret(MIX *mp, uint32_t address){
    Instruction instruction(address);
    mp->A.value = memory[instruction.AA + (instruction.I == 0? 0: mp->I[instruction.I + 1].value)];
}



#include <stdio.h>
int main(){
    MIX mix;
    Instruction instr = Instruction{
        11,
        2,
        2000,
        LDA
    };
    uint64_t op = create_instruction(instr);
    instr = create_instruction(op);

    memory[2000] = 300;
    interpret(&mix, op);

    printf("%ld\n", mix.A.value);
}