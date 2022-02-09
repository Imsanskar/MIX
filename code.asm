ORIG 500
NEXT    LDA 2000
        JMP NEXT

        ;LDA 2000(1:5)
        ;LDA 2000(3:5)
        ;LDA 2000(0:3)
        ;LDA 2000(1:1)
        JMP NEXT_2

NEXT_2  LDA 2000(1:1)
HLT