#ifndef JWASM_READER_H
#define JWASM_READER_H

#include <stdint.h>

namespace jwasm {

    class Reader {
        public:
            Reader(const char* path);

            unsigned char* getCursor();
            void move(int c);

            uint8_t readU1();
            int8_t readS1();
            uint16_t readU2();
            int16_t readS2();
            uint32_t readU4();
            int32_t readS4();
            uint64_t readU8();
            int64_t readS8();

        private:
            unsigned char* data;
    };


}

#endif
