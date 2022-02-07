#pragma once
#include <string_view>
#include "./tokenizer.h"
#include "./instruction.h"
#include <vector>

static Opcode opcodes[] = {
    LDA, LDX, LD1, LD2, LD3, LD4, LD5, LD6, LDAN, LD1N, LD2N, LD3N, LD4N, LD5N, LD6N, LDXN,
    STA, STX, ST1, ST2, ST3, ST4, ST5, ST6, STJ, STZ,
    ADD, SUB, MUL, DIV,
    CMPA, CMPX, CMP1, CMP2, CMP3, CMP4, CMP5, CMP6,

    // jump instruction
    JMP, JSJ, JOV, JNOV, 
    JL, JE, JG, JGE, JNE, JLE,
    JAN, JAZ, JAP, JANN, JNZ, JANP,
    JXN, JXZ, JXP, JXNN, JXNZ, JXNP,
    J1N, J1Z, J1P, J1NN, J1NZ, J1NP,
    J2N, J2Z, J2P, J2NN, J2NZ, J2NP,
    J3N, J3Z, J3P, J3NN, J3NZ, J3NP,
    J4N, J4Z, J4P, J4NN, J4NZ, J4NP,
    J5N, J5Z, J5P, J5NN, J5NZ, J5NP,
    J6N, J6Z, J6P, J6NN, J6NZ, J6NP,
    
    // Increase decrease instruction
    INCA,INCX,INC1,INC2,INC3,INC4,INC5,INC6,
    
    // Increase decrease instruction
    DECA,DECX,DEC1,DEC2,DEC3,DEC4,DEC5,DEC6,

    // shift operation
    SLA, SRA, SLAX, SRAX, SLC, SRC,

    // halt and no op 
    NOP, HLT,
};

struct Parser{
    Tokenizer tokenizer;
    bool isParsing;
    std::vector <uint64_t> parsedItem;
    std::string_view error;
};  


Parser create_parser(std::string_view content){
    Tokenizer tokenizer = create_tokenizer(content.data());
    Parser parser;
    parser.tokenizer = tokenizer;
    return parser;
} 


bool acceptToken(Parser *parser, TokenKind kind, TokenKind *out = NULL){
    if(parser->tokenizer.kind == kind){
        if (out) {
            *out = kind;
        }
        parser->isParsing = tokenize(&parser->tokenizer);
        return true;
    }

    return false;
}


bool expectToken(Parser *parser, TokenKind kind, TokenKind *out = NULL){
    if (acceptToken(parser, kind, out)){
        return true;
    }
    parser->isParsing = false;

    return false;
}

bool parseAddressModifier(Parser *parser, Instruction *instuction){
    if(parser->isParsing){
        uint8_t l;
        uint8_t r;
        if(expectToken(parser, TOKEN_NUMBER)){
            instuction->AA = (uint32_t )parser->tokenizer.value;
        }

        if(acceptToken(parser, TOKEN_COMMA)){
            if(expectToken(parser, TOKEN_NUMBER)){
                instuction->I = parser->tokenizer.value;
            }
        }


        // parse field modifier
        if(acceptToken(parser, TOKEN_LEFT_BRACKET)){
            if(expectToken(parser, TOKEN_NUMBER)){
                l = parser->tokenizer.value; 
            }
            if(expectToken(parser, TOKEN_COLON)){
                // l = parser->tokenizer.value; 
            }
            if(expectToken(parser, TOKEN_NUMBER)){
                r = parser->tokenizer.value; 
            }
            if(expectToken(parser, TOKEN_RIGHT_BRACKET)){
                // check if the bracket if complete
            }
        }

        if(!expectToken(parser, TOKEN_EOI)){
            // check if the value is correct
            // TODO: may be impement some error message here 
            printf("%d\n", parser->tokenizer.lineNumber);
            return false;
        }
        instuction->F = 8 * l + r;

        return true;
    }

    return false;
}

bool parseIncrementDecrement(Parser *parser, Instruction *instruction){
    if(parser->isParsing){
        if(expectToken(parser, TOKEN_EOI)){
            return true;
        }
    }
    return false;
}

bool parserJumpOperation(Parser *parser, Instruction *instruction){
    if(parser->isParsing){
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA = TOKEN_NUMBER;
        }
    }
    return false;
}

bool parserRotateOperation(Parser *parser, Instruction *instruction){
    if(parser->isParsing){
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA = TOKEN_NUMBER;
        }
    }
    return false;
}


bool parse(Parser *parser){
    Instruction is;
    parser->isParsing = tokenize(&parser->tokenizer);
    while(parser->isParsing){
        // load and store instruction
#if 0
        if (acceptToken(parser, TOKEN_LDA)){
            is.op = LDA;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_LD1)){
            is.op = LD1;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD2)){
            is.op = LD2;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD3)){
            is.op = LD3;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD4)){
            is.op = LD4;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD5)){
            is.op = LD5;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD6)){
            is.op = LD6;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LDX)){
            is.op = LDX;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LDAN)){
            is.op = LDAN;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LDXN)){
            is.op = LDXN;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD1N)){
            is.op = LD1N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD2N)){
            is.op = LD2N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD3N)){
            is.op = LD3N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD4N)){
            is.op = LD4N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD5N)){
            is.op = LD5N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD6N)){
            is.op = LD6N;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if (acceptToken(parser, TOKEN_STA)){
            is.op = STA;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_ST1)){
            is.op = ST1;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST2)){
            is.op = ST2;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST3)){
            is.op = ST3;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST4)){
            is.op = ST4;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST5)){
            is.op = ST5;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST6)){
            is.op = ST6;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_STX)){
            is.op = STX;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_STJ)){
            is.op = STJ;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_STZ)){
            is.op = STZ;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        //Arithmetic instructions
        else if(acceptToken(parser, TOKEN_ADD)){
            is.op = ADD;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_SUB)){
            is.op = SUB;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_MUL)){
            is.op = MUL;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DIV)){
            is.op = DIV;
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }else {
                return false;
            }
        }

        // increase decrease instruction
        else if(acceptToken(parser, TOKEN_DECA)){
            is.op = DECA;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DECX)){
            is.op = DECX;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC1)){
            is.op = DEC1;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC2)){
            is.op = DEC2;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC3)){
            is.op = DEC3;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC4)){
            is.op = DEC4;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC5)){
            is.op = DEC5;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_DEC6)){
            is.op = DEC6;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        // increase decrease instruction
        else if(acceptToken(parser, TOKEN_INCA)){
            is.op = INCA;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INCX)){
            is.op = INCX;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC1)){
            is.op = INC1;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC2)){
            is.op = INC2;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC3)){
            is.op = INC3;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC4)){
            is.op = INC4;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC5)){
            is.op = INC5;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_INC6)){
            is.op = INC6;
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
            else {
                return false;
            }
        }

#endif
        if(parser->tokenizer.kind <= TOKEN_DIV){
            is.op = opcodes[parser->tokenizer.kind];
            if(parseAddressModifier(parser, &is)){
                parser->parsedItem.push_back(is);
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_INC6 && parser->tokenizer.kind >= TOKEN_INCA){
            is.op = opcodes[parser->tokenizer.kind];
            if(parseIncrementDecrement(parser, &is)){
                parser->parsedItem.push_back(is);
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_J6NP && parser->tokenizer.kind >=TOKEN_JMP){
            is.op = opcodes[parser->tokenizer.kind];
            if(parserJumpOperation(parser, &is)){
                parser->parsedItem.push_back(is);
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_SRC && parser->tokenizer.kind >=TOKEN_SLA){
            is.op = opcodes[parser->tokenizer.kind];
            if(parserJumpOperation(parser, &is)){
                parser->parsedItem.push_back(is);
            }
        }
        // token end of instruction
        else if(acceptToken(parser, TOKEN_EOI)){

        }
        else if(acceptToken(parser, TOKEN_NOP)){
            is.op = NOP;
            parser->parsedItem.push_back(is);
        }
        else if(acceptToken(parser, TOKEN_HLT)){
            parser->isParsing = false;
            is.op = HLT;
            parser->parsedItem.push_back(is);
            return true;
        }

        else {
            return false;
        }

        // reset instruction 
        is.F = 0;
        is.I = 0;
        is.AA = 0;
    }
    return parser->isParsing;
}