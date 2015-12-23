#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "JavaClassLoader.h"
#include "JavaClass.h"

#define debug

using namespace jwasm;

void mainCompilerStart(const char* name);

int main(int argc, const char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage:\n\tbytecode2wasm inputfile -o outputfile\n");
        return 1;
    }

    // JavaClass javaClass;
    // Reader reader(argv[1]);
    // BytecodeDecoder bytecodedecoder(reader, javaClass);

    // for (std::vector<JavaMethod>::iterator i = javaClass.methods.begin(), 
            // e = javaClass.methods.end(); i !=e; i++) {
    // } 

    mainCompilerStart(argv[1]);

    
    return 0;
}




void mainCompilerStart(const char* name) { 
    JavaClassLoader* loader = new JavaClassLoader();
   
    int size = strlen(name);
    if (size > 4 && (!strcmp(&name[size - 4], ".jar") || !strcmp(&name[size - 6], ".class"))) {



    } else {
    
        char* realName = (char*)malloc(sizeof(char) * (size + 1));
        if (size > 6 && !strcmp(&name[size - 6], ".class")) {
            memcpy(realName, name, size - 6);
            realName[size - 6] = 0;
        } else {
            memcpy(realName, name, size + 1);
        }
            
        const UTF8* utf8 = loader->asciizConstructUTF8(realName);
    
    
    
    }



}
