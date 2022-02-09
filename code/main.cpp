#include <inttypes.h>
#include <stdlib.h>
#include "./tokenizer.h"
#include "./parser.h"
#include "./instruction.h"
#include <vector>
#include "./MIX.h"


        // ;LDA 2000(1:5)
        // ;LDA 2000(3:5)
        // ;LDA 2000(0:3)
        // ;LDA 2000(1:1)


#include <stdio.h>
int main(int argc, char *argv[]){
    const char * ptr = R"foo(
ORIG 500
NEXT    LDA 2000
        JMP NEXT
        JMP NEXT_2

NEXT_2  LDA 2000(1:1)
HLT
    )foo";
    printf("%s\n", ptr);
    MIX mp;
    Parser parser = create_parser(ptr);
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
