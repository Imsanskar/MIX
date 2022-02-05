#include <inttypes.h>
#include <string>

//TODO: Complete remaining instruction and tokens for it
//TODO: Tokenizer

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
    STJ = 32, // STORE THE CONTENTS OF JUMP REGISTER
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
    //Comparison operators
    // sets LESS, GREATER, EQUAL
    CMPA = 56, // C= 56, F = field, if F does not include the sign position the fields are both considered non negative
                // otherewise sign is taken into consideration

    CMPX = 63, // compare X
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
    JXNZ = 47 | (4 << 6),
    JXNP = 47 | (5 << 6), // zero or negative


    // same as above but for X register
    J1N = 41,
    J1Z = 41 | (1 << 6),
    J1P = 41 | (2 << 6),
    J1NN = 41 | (3 << 6),
    J1NZ = 41 | (4 << 6),
    J1NP = 41 | (5 << 6), // zero or negative
    
    J2N = 42,
    J2Z = 42 | (1 << 6),
    J2P = 42 | (2 << 6),
    J2NN = 42 | (3 << 6),
    J2NZ = 42 | (4 << 6),
    J2NP = 42 | (5 << 6), // zero or negative
    
    J3N = 43,
    J3Z = 43 | (1 << 6),
    J3P = 43 | (2 << 6),
    J3NN = 43 | (3 << 6),
    J3NZ = 43 | (4 << 6),
    J3NP = 43 | (5 << 6), // zero or negative
    
    J4N = 44,
    J4Z = 44 | (1 << 6),
    J4P = 44 | (2 << 6),
    J4NN = 44 | (3 << 6),
    J4NZ = 44 | (4 << 6),
    J4NP = 44 | (5 << 6), // zero or negative
    
    J5N = 45,
    J5Z = 45 | (1 << 6),
    J5P = 45 | (2 << 6),
    J5NN = 45 | (3 << 6),
    J5NZ = 45 | (4 << 6),
    J5NP = 45 | (5 << 6), // zero or negative
    
    J6N = 46,
    J6Z = 46 | (1 << 6),
    J6P = 46 | (2 << 6),
    J6NN = 46 | (3 << 6),
    J6NZ = 46 | (4 << 6),
    J6NP = 46 | (5 << 6), // zero or negative

    // Increase decrease instruction
    INCA = 48, // increase A
    INCX = 55, // increase X
    INC1 = 49, 
    INC2 = 50,
    INC3 = 51,
    INC4 = 52,
    INC5 = 53,
    INC6 = 54,

    // Increase decrease instruction
    DECA = 48, // increase A
    DECX = 55, // increase X
    DEC1 = 49, 
    DEC2 = 50,
    DEC3 = 51,
    DEC4 = 52,
    DEC5 = 53,
    DEC6 = 54,


    // shift operators
    SLA = 6, // shift left A
    SRA = 6 | (1 << 6), // shift right A
    SLAX = 6 | (2 << 6), // shift left AX
    SRAX = 6 | (3 << 6), // shift left AX
    SLC = 6 | (4 << 6), // shift left AX circularly
    SRC = 6 | (5 << 6), // shift left AX circularly 

    NOP = 0, 
    HLT = 5 | (2 << 6), // C = 5, F= 2,   

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

enum TokenKind{
    // instruction token
    // load instruction token
    TOKEN_LDA, TOKEN_LDX, TOKEN_LD1, TOKEN_LD2, TOKEN_LD3, TOKEN_LD4, TOKEN_LD5, TOKEN_LD6, TOKEN_LDAN, TOKEN_LD1N, TOKEN_LD2N, TOKEN_LD3N, TOKEN_LD4N, TOKEN_LD5N, TOKEN_LD6N, TOKEN_LDXN,

    // store instruction token
    TOKEN_STA, TOKEN_STX, TOKEN_STA1, TOKEN_STA2, TOKEN_STA3, TOKEN_STA4, TOKEN_STA5, TOKEN_STA6, TOKEN_STJ, TOKEN_STZ,

    // arithmetic instruction token
    TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_DIV,

    // compare instruction
    TOKEN_CMPA, TOKEN_CMPX, TOKEN_CMP1, TOKEN_CMP2, TOKEN_CMP3, TOKEN_CMP4, TOKEN_CMP5, TOKEN_CMP6,

    // Increase decrease instruction
    TOKEN_INCA,TOKEN_INCX,TOKEN_INC1,TOKEN_INC2,TOKEN_INC3,TOKEN_INC4,TOKEN_INC5,TOKEN_INC6,
    
    // Increase decrease instruction
    TOKEN_DECA,TOKEN_DECX,TOKEN_DEC1,TOKEN_DEC2,TOKEN_DEC3,TOKEN_DEC4,TOKEN_DEC5,TOKEN_DEC6,


    // jump instruction
    TOKEN_JMP, TOKEN_JSP, TOKEN_JOV, TOKEN_JNOV, 
    TOKEN_JL, TOKEN_JE, TOKEN_JG, TOKEN_JGE, TOKEN_JNE, TOKEN_JLE,
    TOKEN_JAN, TOKEN_JAZ, TOKEN_JAP, TOKEN_JANN, TOKEN_JNZ, TOKEN_JANP,
    TOKEN_JXN, TOKEN_JXZ, TOKEN_JXP, TOKEN_JXNN, TOKEN_JXNZ, TOKEN_JXNP,
    TOKEN_J1N, TOKEN_J1Z, TOKEN_J1P, TOKEN_J1NN, TOKEN_J1NZ, TOKEN_J1NP,
    TOKEN_J2N, TOKEN_J2Z, TOKEN_J2P, TOKEN_J2NN, TOKEN_J2NZ, TOKEN_J2NP,
    TOKEN_J3N, TOKEN_J3Z, TOKEN_J3P, TOKEN_J3NN, TOKEN_J3NZ, TOKEN_J3NP,
    TOKEN_J4N, TOKEN_J4Z, TOKEN_J4P, TOKEN_J4NN, TOKEN_J4NZ, TOKEN_J4NP,
    TOKEN_J5N, TOKEN_J5Z, TOKEN_J5P, TOKEN_J5NN, TOKEN_J5NZ, TOKEN_J5NP,
    TOKEN_J6N, TOKEN_J6Z, TOKEN_J6P, TOKEN_J6NN, TOKEN_J6NZ, TOKEN_J6NP,

    // shift operation
    TOKEN_SLA, TOKEN_SRA, TOKEN_SLAX, TOKEN_SRAX, TOKEN_SLC, TOKEN_SRC,

    // halt and no op 
    TOKEN_NOP, TOKEN_HLT,

    TOKEN_REG_A, TOKEN_REG_X, TOKEN_REG_I1, TOKEN_REG_I2, TOKEN_REG_I3, TOKEN_REG_I4, TOKEN_REG_I5, TOKEN_REG_I6,

    TOKEN_ID, TOKEN_COLON, TOKEN_NUMBER, TOKEN_COMMA, TOKEN_ERROR, TOKEN_EOI,

};


std::string keywords[] =  {
    "LDA", "LDX", "LD1", "LD2", "LD3", "LD4", "LD5", "LD6", "LDAN", "LD1N", "LD2N", "LD3N", "LD4N", "LD5N", "LD6N", "LDXN",
    "STA", "STX", "STA1", "STA2", "STA3", "STA4", "STA5", "STA6", "STJ", "STZ",

    "ADD", "SUB", "MUL", "DIV",

    "CMPA", "CMPX", "CMP1", "CMP2", "CMP3", "CMP4", "CMP5", "CMP6",

    "JMP", "JSP", "JOV", "JNOV", 
    "JL", "JE", "JG", "JGE", "JNE", "JLE",
    "JAN", "JAZ", "JAP", "JANN", "JNZ", "JANP",
    "JXN", "JXZ", "JXP", "JXNN", "JXNZ", "JXNP",
    "J1N", "J1Z", "J1P", "J1NN", "J1NZ", "J1NP",
    "J2N", "J2Z", "J2P", "J2NN", "J2NZ", "J2NP",
    "J3N", "J3Z", "J3P", "J3NN", "J3NZ", "J3NP",
    "J4N", "J4Z", "J4P", "J4NN", "J4NZ", "J4NP",
    "J5N", "J5Z", "J5P", "J5NN", "J5NZ", "J5NP",
    "J6N", "J6Z", "J6P", "J6NN", "J6NZ", "J6NP",


    // Increase decrease instruction
    "INCA","INCX","INC1","INC2","INC3","INC4","INC5","INC6",
    
    // Increase decrease instruction
    "DECA","DECX","DEC1","DEC2","DEC3","DEC4","DEC5","DEC6",

    "SLA", "SRA", "SLAX", "SRAX", "SLC", "SRC",
    "NOP", "HLT",

    "rA", "rX", "rI1", "rI2", "rI3", "rI4", "rI5", "rI6",
};



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