#include <cstdio>
#include <assert.h>
#include "JavaClass.h"
#include "JavaConstantPool.h"
#include "Reader.h"
#include "debug.h"

#define MAGIC 0xcafebabe
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
CommonJavaClass::~CommonJavaClass() {
//TODO:
}

JavaClass::~JavaClass() {
//TODO:
}

JavaField::~JavaField() {
//TODO:
}

JavaMethod::~JavaMethod() {
//TODO:
}

CommonJavaClass::CommonJavaClass(JavaClassLoader* loader, const UTF8* n) {
    name = n;
    classLoader = loader;
    interfaces_count = 0;
    access = 0;
    super = 0;
}

JavaClass::JavaClass(JavaClassLoader* loader, const UTF8* n, ClassBytes* b) : CommonJavaClass(loader, n) { 
    bytes = b;
    access = JWASM_CLASS;
    virtualFields_count = 0;
    staticFields_count = 0;
    virtualMethods_count = 0;
    staticMethods_count = 0;
    attributes_count = 0;
}

void JavaClass::readClass() {
    std::string clname;
    clname = name->toString(clname);
    PRINT_DEBUG(JWASM_LOAD, 0, LIGHT_GREEN, "readClass \t");
    PRINT_DEBUG(JWASM_LOAD, 0, DARK_MAGENTA, "%s\n", clname.c_str());
    
    Reader reader(bytes);
    uint32_t magic;
    magic = reader.readU4();
    assert(magic == MAGIC && "The magic of the class file is not correct");

    uint16_t minor = reader.readU2();
    uint16_t major = reader.readU2();
    isClassVersionSupported(major, minor);

    uint32_t ctpSize = reader.readU2();
    ctpInfo = new JavaConstantPool(this, reader, ctpSize);
    
    access |= reader.readU2();
    
    if (!isPublic(access)) access |= ACC_PRIVATE;

    const UTF8* thisClassName = ctpInfo->resolveClassName(reader.readU2());
    
}

void JavaClass::getMinimalJDKVersion(uint16_t major, uint16_t minor, uint16_t& JDKMajor, uint16_t& JDKMinor,
        uint16_t& JDKBuild) {
    JDKMajor = 1;
    JDKBuild = 0;
    if (major == 45 && minor <=3) { // Supported by Java 1.0.2
        JDKMinor = 0; 
        JDKBuild = 2;
    } else if (major == 5 && minor <= 65535) { // Supported by Java 1.1
        JDKMinor = 1;
    } else { // Supported by Java 1.x (x >= 2)
        JDKMinor = major - 43; 
        if (minor == 0) --JDKMinor;
    }
}

bool JavaClass::isClassVersionSupported(uint16_t major, uint16_t minor) {
    const uint16_t supportedJavaMinorVersion = 7; // Java 1.7
    
    JavaClass::getMinimalJDKVersion(major, minor, minJDKVersionMajor, minJDKVersionMinor, minJDKVersionBuild);

    bool res = (minJDKVersionMajor <= 1) && (minJDKVersionMinor <= supportedJavaMinorVersion);
    if (!res) {
        std::cerr << "WARNING: Class file '" << *name << "' requires Java version " << minJDKVersionMajor << 
            "." << minJDKVersionMinor << ". This jwasm only supports Java versions up to 1." << 
            supportedJavaMinorVersion << "." << std::endl;
    }
    return res;
}










