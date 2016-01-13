#include <cstdio>
#include <cstdlib>
#include "JavaClass.h"
#include "JavaConstantPool.h"
#include "Reader.h"
#include "debug.h"

#define JWASM_LOAD 4

using namespace jwasm;

const uint32_t JavaConstantPool::ConstantUTF8 = 1;
const uint32_t JavaConstantPool::ConstantInteger = 3;
const uint32_t JavaConstantPool::ConstantFloat = 4;
const uint32_t JavaConstantPool::ConstantLong = 5;
const uint32_t JavaConstantPool::ConstantDouble = 6;
const uint32_t JavaConstantPool::ConstantClass = 7;
const uint32_t JavaConstantPool::ConstantString = 8;
const uint32_t JavaConstantPool::ConstantFieldref = 9;
const uint32_t JavaConstantPool::ConstantMethodref = 10;
const uint32_t JavaConstantPool::ConstantInterfaceMethodref = 11;
const uint32_t JavaConstantPool::ConstantNameAndType = 12;

static uint32_t unimplemented(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    fprintf(stderr, "Unimplemented in constant pool: %d\n", index);
    exit(1);
    return 1;
}

JavaConstantPool::ctpReader JavaConstantPool::funcsReader[16] = {
    unimplemented,
    CtpReaderUTF8,
    unimplemented,
    CtpReaderInteger,
    CtpReaderFloat,
    CtpReaderLong,
    CtpReaderDouble,
    CtpReaderClass,
    CtpReaderString,
    CtpReaderFieldref,
    CtpReaderMethodref,
    CtpReaderInterfaceMethodref,
    CtpReaderNameAndType,
    unimplemented,
    unimplemented,
    unimplemented
};

uint32_t JavaConstantPool::CtpReaderClass(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint16_t entry = reader.readU2();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <class>\tutf8 is at %d\n", index, entry);
    return 1;
}

uint32_t JavaConstantPool::CtpReaderInteger(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t val = reader.readU4();
    ctp->ctpDef[index] = val;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <integer>\tinteger: %d\n", index, val);
    return 1;
}

uint32_t JavaConstantPool::CtpReaderFloat(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t val = reader.readU4();
    ctp->ctpDef[index] = val;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <float>\tfloat: %d\n", index, val);
    return 1;
}

uint32_t JavaConstantPool::CtpReaderUTF8(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    ctp->ctpDef[index] = reader.cursor;
    uint16_t len = reader.readU2();
    char* str = (char*)malloc(sizeof(char) * (len + 1));
    for (uint32_t i = 0; i < len; i++) 
        str[i] = reader.readU1();
    str[len] = 0;
    // reader.cursor += len;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <utf8>\tutf:%s\n", index, str);
    free(str);
    return 1;
}

uint32_t JavaConstantPool::CtpReaderNameAndType(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t entry = reader.readU4();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <name/type>\tname is at %d, type is at %d\n", index, 
            (entry >> 16), (entry & 0xffff));
    return 1;
}

uint32_t JavaConstantPool::CtpReaderFieldref(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t entry = reader.readU4();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <fieldref>\tclass is at %d, name/type is at %d\n", 
            index, (entry >> 16), (entry & 0xffff));
    return 1;
}

uint32_t JavaConstantPool::CtpReaderString(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint16_t entry = reader.readU2();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <string>\tutf8 is at %d\n", index, entry);
    return 1;
}

uint32_t JavaConstantPool::CtpReaderMethodref(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t entry = reader.readU4();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <methodref>\tclass is at %d, name/type is at %d\n", 
            index, (entry >> 16), (entry & 0xffff));
    return 1;
}

uint32_t JavaConstantPool::CtpReaderInterfaceMethodref(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    uint32_t entry = reader.readU4();
    ctp->ctpDef[index] = entry;
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, 
            "; [%5d] <interfaceMethodref>\tclass is at %d, name/type is at %d\n", 
            index, (entry >> 16), (entry & 0xffff));
    return 1;
}

uint32_t JavaConstantPool::CtpReaderLong(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    ctp->ctpDef[index + 1] = reader.readU4();
    ctp->ctpDef[index] = reader.readU4();
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <long>\t long:%d %d\n", index, 
            ctp->ctpDef[index], ctp->ctpDef[index + 1]);
    return 2;
}

uint32_t JavaConstantPool::CtpReaderDouble(JavaConstantPool* ctp, Reader& reader, uint32_t index) {
    ctp->ctpDef[index + 1] = reader.readU4();
    ctp->ctpDef[index] = reader.readU4();
    PRINT_DEBUG(JWASM_LOAD, 0, COLOR_NORMAL, "; [%5d] <double>\t double:%d %d\n", index, 
            ctp->ctpDef[index], ctp->ctpDef[index + 1]);
    return 2;
}

JavaConstantPool::JavaConstantPool(JavaClass* cl, Reader& reader, uint32_t size) {
    ctpSize = size;
    classDef = cl;
    
    // store type
    ctpType = new uint8_t[ctpSize];
    // store data
    ctpDef = new int32_t[ctpSize];

    uint32_t cur = 1;
    while (cur < ctpSize) {
        uint8_t curType = reader.readU1();
        ctpType[cur] = curType;
        cur += ((funcsReader[curType])(this, reader, cur));
    }
}

JavaConstantPool::~JavaConstantPool() {

}
