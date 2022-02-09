#pragma once
#include <string_view>
#include "./tokenizer.h"
#include "./instruction.h"
#include <vector>
#include <stdio.h>
#include "MIX.h"
#include <unordered_map>

static Opcode opcodes[] = {
    LDA, LDX, LD1, LD2, LD3, LD4, LD5, LD6, LDAN, LD1N, LD2N, LD3N, LD4N, LD5N, LD6N, LDXN,
    STA, STX, ST1, ST2, ST3, ST4, ST5, ST6, STJ, STZ,
    ADD, SUB, MUL, DIV,
    CMPA, CMPX, CMP1, CMP2, CMP3, CMP4, CMP5, CMP6,

    ENTA, ENT1, ENT2, ENT3, ENT4, ENT5, ENT6, ENTX,
    ENNA, ENN1, ENN2, ENN3, ENN4, ENN5, ENN6, ENNX,

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

char error[1024];
std::unordered_map<std::string, int32_t> unfound_labels;

struct Parser{
    Tokenizer tokenizer;
    bool isParsing;
    std::vector <Memory> parsedItem;
    std::string_view error;
    std::unordered_map<std::string, uint32_t> labels;
};  


Parser create_parser(std::string_view content){
    Tokenizer tokenizer = create_tokenizer(content.data());
    Parser parser;
    parser.tokenizer = tokenizer;
    return parser;
} 

bool getToken(Parser *parser){
    parser->isParsing = tokenize(&parser->tokenizer);

    return parser->isParsing;
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

bool parseAddressModifier(Parser *parser, Memory *instruction){
    if(parser->isParsing){
        uint8_t l;
        uint8_t r;
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA = (uint32_t )parser->tokenizer.value;
        }

        if(acceptToken(parser, TOKEN_COMMA)){
            if(expectToken(parser, TOKEN_NUMBER)){
                instruction->I = parser->tokenizer.value;
            }
        }


        // parse field modifier
        if(acceptToken(parser, TOKEN_LEFT_BRACKET)){
            if(expectToken(parser, TOKEN_NUMBER)){
                l = parser->tokenizer.value; 
            }
            else {
                sprintf(error, "Syntax error: Expected number got something else, LINE: %d, column: %d", parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
                return false;
            }
            if(expectToken(parser, TOKEN_COLON)){
                // l = parser->tokenizer.value; 
            }
            else {
                sprintf(error, "Syntax error: Expected colon got something else, LINE: %d, column: %d", parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
                return false;
            }
            if(expectToken(parser, TOKEN_NUMBER)){
                r = parser->tokenizer.value; 
            }
            else {
                return false;
            }
            if(expectToken(parser, TOKEN_RIGHT_BRACKET)){
                // check if the bracket if complete
            }
            else {
                sprintf(error, "Syntax error: Unmatched bracket, LINE: %d, column: %d", parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
                return false;
            }
        }

        if(!expectToken(parser, TOKEN_EOI)){
            // check if the value is correct
            // TODO: may be impement some error message here 
            sprintf(error, "Syntax error: Expected Newline got %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
            return false;
        }
        instruction->F = 8 * l + r;

        return true;
    }

    return false;
}

bool parseIncrementDecrement(Parser *parser, Memory *instruction, uint8_t no_of_bits = 2){
    if(parser->isParsing){
        if(acceptToken(parser, TOKEN_MINUS)){
            // 1 in bit 12 means negative operand
            instruction->AA = instruction->AA | (1 << 12);
        }
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA |= (uint32_t )parser->tokenizer.value;
        }
        else{
            sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
        }
        if(acceptToken(parser, TOKEN_COMMA)){
            if(expectToken(parser, TOKEN_NUMBER)){
                instruction->I = parser->tokenizer.value;
            }
            else {
                sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
            }
        }
        if(expectToken(parser, TOKEN_EOI)){
            return true;
        }
        else{
            sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
        }
    }
    return false;
}

bool parserJumpOperation(Parser *parser, Memory *instruction, int32_t address){
    if(parser->isParsing){
        // getToken(parser);
        if(parser->tokenizer.kind == TOKEN_NUMBER){
            instruction->AA = TOKEN_NUMBER;
        }
        else if(parser->tokenizer.kind == TOKEN_ID){
            if(parser->labels.find(parser->tokenizer.id) != parser->labels.end()){
                instruction->AA = parser->labels[parser->tokenizer.id];
            }
            else{
                unfound_labels[parser->tokenizer.id] = address;
            }
        }
        else{
            sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
        }
        getToken(parser);
        if(acceptToken(parser, TOKEN_COMMA)){
            if(expectToken(parser, TOKEN_NUMBER)){
                instruction->I = parser->tokenizer.value;
            }
        }

        if(!expectToken(parser, TOKEN_EOI)){
            sprintf(error, "Syntax error: Expected Newline got %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
            return false;
        }

        return true;
    }
    return false;
}

bool parserRotateOperation(Parser *parser, Memory *instruction){
    if(parser->isParsing){
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA = TOKEN_NUMBER;
        }
        else{
            sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
        }
    }
    return false;
}


bool parseAddressTransferInstruction(Parser *parser, Memory *instruction){
    if(parser->isParsing){
        if(acceptToken(parser, TOKEN_MINUS)){
            // 1 in bit 31 means negative
            instruction->AA = instruction->AA | (1 << 30);
        }
        if(expectToken(parser, TOKEN_NUMBER)){
            instruction->AA |= (uint32_t )parser->tokenizer.value;
        }
        else  {
            sprintf(error, "Syntax error: %s, LINE: %d, column: %d", parser->tokenizer.id.c_str(), parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
        }
        if(acceptToken(parser, TOKEN_COMMA)){
            if(expectToken(parser, TOKEN_NUMBER)){
                instruction->I = parser->tokenizer.value;
            }
        }
        

    }

    return false;
}


bool parse(Parser *parser, MIX *mp){
    Memory is;
    parser->isParsing = tokenize(&parser->tokenizer);
    int32_t initial_address = 0;
    while(parser->isParsing){

        // labels can be only at the beginning of the line
        const std::string label = parser->tokenizer.id;
        // original address
        if(acceptToken(parser, TOKEN_ORIG)){
            if(expectToken(parser, TOKEN_NUMBER)){
                if (parser->tokenizer.value > 4095) {
                    sprintf(error, "Syntax error: address can't be greater than 4095, LINE: %d, column: %d", parser->tokenizer.columnNumber, parser->tokenizer.lineNumber);
                    return false;
                }
            }
            initial_address = parser->tokenizer.value;
            mp->pc = initial_address;
        }
        // load and store instruction
        else if(parser->tokenizer.kind <= TOKEN_CMP6){
            is.op = opcodes[parser->tokenizer.kind];
            getToken(parser);
            if(parseAddressModifier(parser, &is)){
                mp->memory[initial_address++] = is;
            }
            else {
                return false;
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_DEC6 && parser->tokenizer.kind >= TOKEN_INCA){
            is.op = opcodes[parser->tokenizer.kind];
            getToken(parser);
            if(parseIncrementDecrement(parser, &is)){
                mp->memory[initial_address++] = is;
            }
            else {
                return false;
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_J6NP && parser->tokenizer.kind >=TOKEN_JMP){
            is.op = opcodes[parser->tokenizer.kind];
            getToken(parser);
            is.F = opcodes[parser->tokenizer.kind] >> 6;
            if(parserJumpOperation(parser, &is, initial_address)){
                mp->memory[initial_address++] = is;
            }
            else {
                return false;
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_SRC && parser->tokenizer.kind >=TOKEN_SLA){
            is.op = opcodes[parser->tokenizer.kind];
            getToken(parser);
            if(parserRotateOperation(parser, &is)){
                mp->memory[initial_address++] = is;
            }
            else {
                return false;
            }
        }
        else if(parser->tokenizer.kind <= TOKEN_ENNX && parser->tokenizer.kind >= TOKEN_ENTA){
            is.op = opcodes[parser->tokenizer.kind];
            is.F = opcodes[parser->tokenizer.kind] >> 6;
            // getToken(parser);
            if(parseAddressTransferInstruction(parser, &is)){
                mp->memory[initial_address++] = is;
            }
            else {
                return false;
            }
        }
        // token end of instruction
        else if(acceptToken(parser, TOKEN_EOI)){
            
        }
        else if(acceptToken(parser, TOKEN_ID)){
                if(parser->labels.find(label) == parser->labels.end()){
                    parser->labels[label] = initial_address;

                }
                else{
                    sprintf(error, "Multiple lables defined, LINE: %d, column: %d", parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
                    return false;
                }
        }
        else if(acceptToken(parser, TOKEN_NOP)){
            is.op = NOP;
            mp->memory[initial_address++] = is;
        }
        else if(acceptToken(parser, TOKEN_HLT)){
            parser->isParsing = false;
            is.op = HLT;
            mp->memory[initial_address++] = is;
            break;
        }
        else if (acceptToken(parser, TOKEN_ERROR)){
            sprintf(error, "Unidentified character, LINE: %d, column: %d", parser->tokenizer.lineNumber, parser->tokenizer.columnNumber);
            return false;
        }
        else {
            break;
        }


        // reset instruction 
        is.F = 0;
        is.I = 0;
        is.AA = 0;
    }

    // resolve Jump labels
    for(auto pair: unfound_labels){
        if(parser->labels.find(pair.first) != parser->labels.end()){
            mp->memory[pair.second].AA = parser->labels[pair.first];
        }
    }
    return true;
}