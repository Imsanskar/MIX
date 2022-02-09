#include <inttypes.h>
#include <stdlib.h>
#include "./tokenizer.h"
#include "./parser.h"
#include "./instruction.h"
#include <vector>
#include "./MIX.h"

char * read_from_file(const char *file_name, char *code) {
    FILE * file = fopen(file_name, "r");
    
    if(!file) {
        printf("Could not open file\n");
        exit(-1);
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    code  = (char *)malloc(fsize + 2);
    fread(code, fsize, 1, file);

    fclose(file);
    code[fsize] = '\0';

    return code;
}



#include <stdio.h>
int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Usage: MIX <filename>");
    }
    char *code;
    code = read_from_file(argv[1], code);
    
    printf("%s\n", code);
    MIX mp;
    Parser parser = create_parser(code);
    bool isSuccessful = parse(&parser, &mp);
    mp.memory[2000] = Memory(5, 3, -80, (Opcode) 4);
    
    if(!isSuccessful){
        printf("ERROR\n");
        printf("%s\n", error);
        exit(1);
    }
    interpret(&mp, parser.parsedItem);
    Memory m = mp.memory[mp.pc];
    int i = 0;
    while(m.op != HLT & 63){
        m = mp.memory[mp.pc + i];
        printf("%d %d %d %d\n", m.AA, m.I, m.F, m.op);
        i++;
    }
}
