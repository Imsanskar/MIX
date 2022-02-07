#pragma once
#include <string_view>
#include "./tokenizer.h"
#include "./instruction.h"
#include <vector>

struct Parser{
    Tokenizer tokenizer;
    bool isParsing;
    std::vector <uint64_t> parsedItem;
    int line_number = 0;
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
            return false;
        }
        instuction->F = 8 * l + r;

        return true;
    }

    return false;
}

bool parseIncrementDecrement(Parser *parser, Instruction *Instruction){
    if(parser->isParsing){
        if(expectToken(parser, TOKEN_EOI)){
            return true;
        }
    }
    return false;
}


bool parse(Parser *parser){
    Instruction is;
    parser->isParsing = tokenize(&parser->tokenizer);
    while(parser->isParsing){
        // load and store instruction
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



        // token end of instruction
        else if(acceptToken(parser, TOKEN_EOI)){
            parser->line_number += 1; 
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