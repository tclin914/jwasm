#include <stdio.h>
#include <vector>

#include "Reader.h"
#include "BytecodeDecoder.h"
#include "ClassFile.h"

#define debug

using namespace jwasm;

int main(int argc, const char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage:\n\tbytecode2wasm inputfile -o outputfile\n");
        return 1;
    }

    JavaClass javaClass;
    Reader reader(argv[1]);
    BytecodeDecoder bytecodedecoder(reader, javaClass);

    for (std::vector<JavaMethod>::iterator i = javaClass.methods.begin(), 
            e = javaClass.methods.end(); i !=e; i++) {

    } 
    return 0;
}
