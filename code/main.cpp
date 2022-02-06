#include <inttypes.h>
#include "./tokenizer.h"
#include "./parser.h"
#include "./instruction.h"
//TODO: parser
// look into field modifier stuffs

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



uint64_t memory[4095];

void interpret(MIX *mp, uint32_t address){
    Instruction instruction(address);
    mp->A.value = memory[instruction.AA + (instruction.I == 0? 0: mp->I[instruction.I + 1].value)];
}



#include <stdio.h>
int main(){
    const char * ptr = R"foo(
        LDA 2000, 20:3)
        STa 2000; hello there comment
        STA 2000, 2; some comment
        NOP
        HLT
    )foo";
    Parser parser = create_parser(ptr);
    bool isSuccessful = parse(&parser);
    for(Instruction ins: parser.parsedItem){
        int x = 0;
    }
}