#include <cstdio>
#include "JavaClass.h"
#include "debug.h"

#define JWASM_LOAD 4

using namespace jwasm;

JavaAttribute::JavaAttribute(const UTF8* name, uint32_t length, uint32_t offset) {
    this->start = offset;
    this->size = length;
    this->name = name;
}

JavaAttribute* JavaClass::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = attributes[i];
        if (cur->name->equals(key)) return cur;
    }
    return NULL;
}

JavaAttribute* JavaField::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = attributes[i];
        if (cur->name->equals(key)) return cur;
    }
    return NULL;
}

JavaAttribute* JavaMethod::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = attributes[i];
        if (cur->name->equals(key)) return cur;
    }
    return NULL;
}

//TODO:destructor
CommonJavaClass::~CommonJavaClass() {
}

JavaClass::~JavaClass() {
}

JavaField::~JavaField() {
}

JavaMethod::~JavaMethod() {
}

CommonJavaClass::CommonJavaClass(JavaClassLoader* loader, const UTF8* n) {
    name = n;
    classLoader = loader;
    interfaces_count = 0;
    access = 0;
    super = 0;
}

JavaClass::JavaClass(JavaClassLoader* loader, const UTF8* n, ClassBytes* bytes) : CommonJavaClass(loader, n) { 
    access = JWASM_CLASS;
    virtualFields_count = 0;
    staticFields_count = 0;
    virtualMethods_count = 0;
    staticMethods_count = 0;
    attributes_count = 0;
}

void JavaClass::readClass() {
    PRINT_DEBUG(JWASM_LOAD, 0, LIGHT_GREEN, "readClass \t\n");
    // PRINT_DEBUG(JWASM_LOAD, 0, DARK_MAGENTA, "%s\n", );

}










