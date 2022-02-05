SRC_FILES="../code/main.cpp ../code/lexer.h"

if [ ! -d "bin/" ]
then 
    mkdir -p bin
fi

cd bin/

g++ -g -Wall $SRC_FILES -o MIX


cd ../