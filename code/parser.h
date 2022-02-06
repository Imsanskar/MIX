#pragma once
#include <string_view>
#include "./tokenizer.h"
#include "./instruction.h"
#include <vector>

struct Parser{
    Tokenizer tokenizer;
    bool isParsing;
    std::vector <uint64_t> parsedItem;
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

bool parseLoadStoreInstuction(Parser *parser, Instruction *instuction){
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
        instuction->F = 8 * l + r;

        return true;
    }

    return false;
}


bool parse(Parser *parser){
    Instruction is;
    parser->isParsing = tokenize(&parser->tokenizer);
    while(parser->isParsing){
        if (acceptToken(parser, TOKEN_LDA)){
            is.op = LDA;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_LD1)){
            is.op = LD1;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD2)){
            is.op = LD2;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD3)){
            is.op = LD3;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD4)){
            is.op = LD4;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD5)){
            is.op = LD5;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LD6)){
            is.op = LD6;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LDX)){
            is.op = LDX;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_LDAN)){
            is.op = LDAN;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if (acceptToken(parser, TOKEN_STA)){
            is.op = STA;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }
        }
        else if(acceptToken(parser, TOKEN_ST1)){
            is.op = ST1;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST2)){
            is.op = ST2;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST3)){
            is.op = ST3;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST4)){
            is.op = ST4;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST5)){
            is.op = ST5;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST6)){
            is.op = ST6;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_STX)){
            is.op = STX;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
        else if(acceptToken(parser, TOKEN_ST1)){
            is.op = ST1;
            if(parseLoadStoreInstuction(parser, &is)){
                parser->parsedItem.push_back(is);
            }else{
                return false;
            }

        }
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