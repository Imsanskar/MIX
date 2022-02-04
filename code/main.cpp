#include <inttypes.h>


struct Register{
    uint64_t value;
    const uint8_t noOfBytes;
    uint8_t sign = 0;

    Register(uint64_t _value, uint8_t bytes, uint8_t _sign): noOfBytes(bytes){
        value = _value;
        sign = _sign;
    }

    Register(uint8_t bytes = 2): noOfBytes(bytes){
    }
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

};

enum Opcode{
    //load instructions
    LDA = 8, // load into accumulator
    LDX = 15, // load into X register
    LD1 = 9, // 
    LD2 = 10,
    LD3 = 11,
    LD4 = 12,
    LD5 = 13,
    LD6 = 14,
    LDAN = 16, // load into accumulator
    LD1N = 17, 
    LD2N = 18,
    LD3N = 19,
    LD4N = 20,
    LD5N = 21,
    LD6N = 22,
    LDXN = 23, // load into X register but negative

    // Storing operation
    STA = 24,
    STX = 31,
    STA1 = 25, // 
    STA2 = 26,
    STA3 = 27,
    STA4 = 28,
    STA5 = 29,
    STA6 = 30,
    STJ = 32,
    STZ = 33, // clears the content of the memory


    // Arthmetic instruction
    // field specification can be used, a field specificcation of (0:6) can be used to indicate floating point operation
    ADD = 1, // A <- A + operand
    SUB = 2, // A <- A - operand
    MUL = 3, // A, X <- A * operand
    //TODO: Check division later
    DIV = 4, // AX treated as a single number with the sign of A, divided value of operand 


    //Comparison operator
    // when F = (0:0) equal operation occur because -0 = +0
    CMPA = 56,
    CMPX = 63,
    CMP1 = 57,
    CMP2 = 58,
    CMP3 = 59,
    CMP4 = 60,
    CMP5 = 61,
    CMP6 = 62,


    // Jump Operation
    JMP = 39, // unconditional jump
    JSJ = 39 | (1 << 6), // jump and save the content in the J register
    JOV = 39 | (2 << 6), // if overflow is turned on then it is turned of and JMP occurs
    JNOV = 39 | (3 << 6), // if overflow is turned off JUMP occurs, otherwise it is turned off
    // jump of less, equal, greater, greater equal, not equal, less or equal
    JL = 39 | (4 << 6), 
    JE = 39 | (5 << 6),
    JG = 39 | (6 << 6),
    JGE = 39 | (7 << 6),
    JNE = 39 | (8 << 6),
    JLE = 39 | (9 << 6),

    // jump A negative, zero, positive, non negative, non zero, non positive
    JAN = 40,
    JAZ = 40 | (1 << 6),
    JAP = 40 | (2 << 6),
    JANN = 40 | (3 << 6),
    JNZ = 40 | (4 << 6),
    JANP = 40 | (5 << 6), // zero or negative

    // same as above but for X register
    JXN = 47,
    JXZ = 47 | (1 << 6),
    JXP = 47 | (2 << 6),
    JXNN = 47 | (3 << 6),
    JXZ = 47 | (4 << 6),
    JXNP = 47 | (5 << 6), // zero or negative


    // same as above but for X register
    J1N = 41,
    J1Z = 41 | (1 << 6),
    J1P = 41 | (2 << 6),
    J1NN = 41 | (3 << 6),
    J1Z = 41 | (4 << 6),
    J1NP = 41 | (5 << 6), // zero or negative
    
    J2N = 42,
    J2Z = 42 | (1 << 6),
    J2P = 42 | (2 << 6),
    J2NN = 42 | (3 << 6),
    J2Z = 42 | (4 << 6),
    J2NP = 42 | (5 << 6), // zero or negative
    
    J3N = 43,
    J3Z = 43 | (1 << 6),
    J3P = 43 | (2 << 6),
    J3NN = 43 | (3 << 6),
    J3Z = 43 | (4 << 6),
    J3NP = 43 | (5 << 6), // zero or negative
    
    J4N = 44,
    J4Z = 44 | (1 << 6),
    J4P = 44 | (2 << 6),
    J4NN = 44 | (3 << 6),
    J4Z = 44 | (4 << 6),
    J4NP = 44 | (5 << 6), // zero or negative
    
    J5N = 45,
    J5Z = 45 | (1 << 6),
    J5P = 45 | (2 << 6),
    J5NN = 45 | (3 << 6),
    J5Z = 45 | (4 << 6),
    J5NP = 45 | (5 << 6), // zero or negative
    
    J6N = 46,
    J6Z = 46 | (1 << 6),
    J6P = 46 | (2 << 6),
    J6NN = 46 | (3 << 6),
    J6Z = 46 | (4 << 6),
    J6NP = 46 | (5 << 6), // zero or negative

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