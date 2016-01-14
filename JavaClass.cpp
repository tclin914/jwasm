#include <cstdio>
#include <cstdlib>
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
        JavaAttribute* cur = &attributes[i];
        if (cur->name->equals(key)) return cur;
    }
    return NULL;
}

JavaAttribute* JavaField::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = &attributes[i];
        if (cur->name->equals(key)) return cur;
    }
    return NULL;
}

JavaAttribute* JavaMethod::lookupAttribute(const UTF8* key) {
    for (uint32_t i = 0; i < attributes_count; ++i) {
        JavaAttribute* cur = &attributes[i];
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

JavaField::JavaField(JavaClass* cl, const UTF8* N, const UTF8* T, uint16_t A) {
    name = N;
    type = T;
    classDef = cl;
    access = A;
}

JavaAttribute* JavaClass::readAttributes(Reader& reader, uint16_t& size) {
    uint16_t nbA = reader.readU2();

    JavaAttribute* attributes = new JavaAttribute[nbA];

    for (int i = 0; i < nbA; i++) {
        const UTF8* name = ctpInfo->UTF8At(reader.readU2());
        uint32_t len = reader.readU4();
        JavaAttribute& att = attributes[i];
        att.start = reader.cursor;
        att.size = len;
        att.name = name;
        reader.seek(len, SEEK_CUR);
    }
    size = nbA;
    return attributes;
}

void JavaClass::readFields(Reader& reader) {
    uint16_t nbFields = reader.readU2();
    for (int i = 0; i < nbFields; i++) {
        uint16_t access = reader.readU2();
        const UTF8* name = ctpInfo->UTF8At(reader.readU2());
        const UTF8* type = ctpInfo->UTF8At(reader.readU2());
        JavaField* field = new JavaField(this, name, type, access);
        if (isStatic(access)) {
            staticFields.push_back(field);
            staticFields_count++;
        } else {
            virtualFields.push_back(field);
            virtualFields_count++;
        }
        // field->attributes = readAttributes(reader, field->attributes_count);
    }
}

void JavaClass::readParents(Reader& reader) {
    uint16_t superEntry = reader.readU2();
    if (superEntry) {
        const UTF8* superUTF8 = ctpInfo->resolveClassName(superEntry);
        // TODO:not yet set class path for searching parent class
        // super = classLoader->loadName(superUTF8, false, true);
        // super->name->dump();
    }

    uint16_t interfaces_count = reader.readU2();
    interfaces.resize(interfaces_count);

    for (int i = 0; i < interfaces_count; i++) {
        const UTF8* name = ctpInfo->resolveClassName(reader.readU2());
        interfaces[i] = classLoader->loadName(name, false, true);
    }
}


void JavaClass::readClass() {
    PRINT_DEBUG(JWASM_LOAD, 0, LIGHT_GREEN, "readClass \t");
    PRINT_DEBUG(JWASM_LOAD, 0, DARK_MAGENTA, "%s\n", UTF8Buffer(name).cString());
    
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

    if (!(thisClassName->equals(name))) {
        fprintf(stderr, "noClassDefFoundError\n");
        exit(1);
    }

    readParents(reader);
    // readFields(reader);
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


