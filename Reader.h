#ifndef JWASM_READER_H
#define JWASM_READER_H

#include <stdint.h>

#include "JavaClass.h"

namespace jwasm {
    
    class ClassBytes {
        public:
            ClassBytes(int l) {
                size = l;
            }

            uint32_t size;
            uint8_t elements[1];
    };

    class Reader {
        public:
            ClassBytes* bytes;
            uint32_t min;
            uint32_t cursor;
            uint32_t max;

            Reader(JavaAttribute* attr, ClassBytes* bytes) {
                this->bytes = bytes;
                this->cursor = attr->start;
                this->min = attr->start;
                this->max = attr->start + attr->size;
            }

            Reader(Reader& r, uint32_t size) {
                bytes = r.bytes;
                cursor = r.cursor;
                min = r.min;
                max = min + size;
            }

            Reader(ClassBytes* array, uint32_t start = 0, uint32_t end = 0) {
                if (!end) end = array->size;
                this->bytes = array;
                this->cursor = start;
                this->min = start;
                this->max = start + end;
            }

            static ClassBytes* openFile(JavaClassLoader* loader, const char* path);

            uint8_t readU1();
            int8_t readS1();
            uint16_t readU2();
            int16_t readS2();
            uint32_t readU4();
            int32_t readS4();
            uint64_t readU8();
            int64_t readS8();

            unsigned int tell() {
                return cursor - min;
            }

            void seek(uint32_t pos, int from);
    };


}

#endif
