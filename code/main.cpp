#include <inttypes.h>
#include <string>
#include "./tokenizer.h"

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



uint64_t memory[4095];

void interpret(MIX *mp, uint32_t address){
    Instruction instruction(address);
    mp->A.value = memory[instruction.AA + (instruction.I == 0? 0: mp->I[instruction.I + 1].value)];
}



#include <stdio.h>
int main(){
    const char * ptr = R"foo(
        LDA 2000, 2(0:3)
        STA 2000; hello there comment
        ADD -1000
        JMP 0
        NOP
        HLT
    )foo";
    Tokenizer tokenizer = create_tokenizer(ptr);
    while(tokenize(&tokenizer)){
        if (tokenizer.kind == TOKEN_ERROR) {
			printf("\nERROR: %s\n", tokenizer.id.c_str());
			break;
		}
		else if (tokenizer.kind == TOKEN_EOI) {
			printf("\n");
		}
		else if (tokenizer.kind == TOKEN_NUMBER) {
			printf("%ld ", tokenizer.value);
		}
		else if (tokenizer.kind == TOKEN_COLON) {
			printf(": ");
		}
		else if (tokenizer.kind == TOKEN_COMMA) {
			printf(", ");
		}
		else if (tokenizer.kind == TOKEN_LEFT_BRACKET) {
			printf("( ");
		}
		else if (tokenizer.kind == TOKEN_RIGHT_BRACKET) {
			printf(") ");
		}
		else if (tokenizer.kind == TOKEN_DOLLAR) {
			printf("$ ");
		}
		else if (tokenizer.kind == TOKEN_MINUS) {
			printf("- ");
		}
		else if (tokenizer.kind == TOKEN_ID) {
			printf("%d %s ", (int32_t)tokenizer.id.size(), tokenizer.id.c_str());
		}
		else {
			printf("%.*s ", (int)keywords[tokenizer.kind].length(), keywords[tokenizer.kind].data());
		} 
    }
}