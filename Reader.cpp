#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reader.h"


using namespace jwasm;

Reader::Reader(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Can not open %s\n", path);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data = (unsigned char*)malloc(fsize);

    if (!data) {
        fprintf(stderr, "Unable to alloc %zd bytes\n", fsize);
        exit(1);
    }
    if (fread(data, 1, fsize, fp) != fsize) {
        fprintf(stderr, "Unable to read %zd bytes from %s\n", fsize, path);
        exit(1);
    }
}

unsigned char* Reader::getCursor() {
    return data;
}

void Reader::move(int c) {
    data += c;
}

uint8_t Reader::readU1() {
    ++data;
    return *(data - 1);
}

int8_t Reader::readS1() {
    ++data;
    return *(data - 1);
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











