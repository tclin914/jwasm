
#include "JavaClass.h"

JavaAttribute::JavaAttribute(const UTF8* name, uint32_t length, uint32_t offset) {
    this->start = offset;
    this->size = length;
    this->name = name;
}

JavaAttribute* JavaClass::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = &(attributes[i]);
        if (strcmp(cur->name, key) == 0) return cur;
    }
}

JavaAttribute* JavaField::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = &(attributes[i]);
        if (strcmp(cur->name, key) == 0) return cur;
    }
}

JavaAttribute* JavaMethod::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = &(attributes[i]);
        if (strcmp(cur->name, key) == 0) return cur;
    }
}

//TODO:destructor
CommonJavaClass::~CommonJavaClass() {
}

Class::~Class() {
}

JavaField::~JavaField() {
}

JavaMethod::~JavaMethod() {
}

CommonJavaClass::CommonJavaClass(JwasmClassLoader* loader, const UTF8* n) {
    name = n;
    classLoader = loader;
    interfaces_count = 0;
    access = 0;
    super = 0;
}

JavaClass::JavaClass(JwasmClassLoader* loader, const UTF8* n) : CommonJavaClass(loader, n) { 
    access = JWASM_CLASS;
    virtualFields_count = 0;
    staticFields_count = 0;
    virtualMethods_count = 0;
    staticMethods_count = 0;
    attributes_count = 0
}





