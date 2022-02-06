SRC_FILES="../code/main.cpp ../code/tokenizer.h"
GCC_FLAGS="-g -std=c++17 -pedantic"

if [ ! -d "bin/" ]
then 
    mkdir -p bin
fi

cd bin/

g++ $GCC_FLAGS $SRC_FILES -o MIX


cd ../