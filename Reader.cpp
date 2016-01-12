#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include "Reader.h"

using namespace jwasm;

ClassBytes* Reader::openFile(JavaClassLoader* loader, const char* path) {
    ClassBytes* res = NULL;
    FILE* fp = fopen(path, "r");
    if (fp != 0) {
        fseek(fp, 0, SEEK_END);
        long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        res = new ClassBytes(len);
        if (fread(res->elements, len, 1, fp) == 0) {
            fprintf(stderr, "fread error\n");
            exit(1);
        }
        fclose(fp);
    }
    return res;
}

void Reader::seek(uint32_t pos, int from) {
    uint32_t n = 0;
    uint32_t start = min;
    uint32_t end = max;

    if (from == SEEK_CUR) n = cursor + pos;
    else if (from == SEEK_SET) n = start + pos;
    else if (from == SEEK_END) n = end + pos;

    assert(n >= start && n <=end && "out fo range");
    cursor = n;
}

uint8_t Reader::readU1() {
    ++cursor;
    return bytes->elements[cursor - 1];
}

int8_t Reader::readS1() {
    ++cursor;
    return bytes->elements[cursor - 1];
}

uint16_t Reader::readU2() {
    uint16_t tmp = ((uint16_t)readU1()) << 8;
    return tmp | ((uint16_t)readU1());
}

int16_t Reader::readS2() {
    int16_t tmp = ((int16_t)readS1()) << 8;
    return tmp | ((int16_t)readU1()) << 8;
}

uint32_t Reader::readU4() {
    uint32_t tmp = ((uint32_t)readU2()) << 16;
    return tmp | ((uint32_t)readU2());
}

int32_t Reader::readS4() {
    int32_t tmp = ((int32_t)readS2()) << 16;
    return tmp | ((int32_t)readU2());
}

uint64_t Reader::readU8() {
    uint64_t tmp = ((uint64_t)readU4()) << 32;
    return tmp | ((uint64_t)readU4());
}

int64_t Reader::readS8() {
    int64_t tmp = ((int64_t)readS4()) << 32;
    return tmp | ((int64_t)readU4());
}

