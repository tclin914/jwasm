#ifndef JWASM_BYTECODE_DECODER_H
#define JWASM_BYTECODE_DECODER_H

#include <vector>

#include "Reader.h"
#include "ConstantPool.h"
#include "ClassFile.h"
#include <vector>

namespace jwasm {
    class BytecodeDecoder {
        public:
            BytecodeDecoder(Reader& reader, JavaClass& javaClass);

            static void resolveAttrubutes(std::vector<JavaAttribute>& attributes, Reader& reader,
                    uint16_t& count);
        private:
            Reader& reader;
            JavaClass& javaClass;
            
            void checkMagicNumber();
            void readVersion();

            void readAccess();
            void readThis();
            void readSuper();

            void readConstantPool();
            void readInterfaces();
            void readFields();
            void readMethods();
            void readAttributes();
    };

}

#endif
