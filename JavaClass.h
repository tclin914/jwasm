#ifndef JWASM_JAVACLASS_H
#define JWASM_JAVACLASS_H

#include <stdint.h>
#include <vector>

#include "UTF8.h"
#include "JavaAccess.h"
#include "JavaClassLoader.h"

namespace jwasm {

    class JavaClass;
    class JavaField;
    class JavaMethod;
    class JavaAttribute;
    class JavaConstantPool;
    class Reader;


    class JavaAttribute {
        public:
            JavaAttribute(const UTF8* name, uint32_t length, uint32_t offset);
            
            const UTF8* name;
            uint32_t start;
            uint32_t size;
    };

    class CommonJavaClass {
        public:            
            CommonJavaClass(JavaClassLoader* loader, const UTF8* name);
            ~CommonJavaClass();

            uint32_t access;

            uint16_t interfaces_count;
            std::vector<JavaClass*> interfaces;

            // The name of the class
            const UTF8* name;

            // The parent of this class
            JavaClass* super;

            JavaClassLoader* classLoader;

            uint32_t getAccess() const { return access & 0xffff; }
            std::vector<JavaClass*> getInterfaces() const { return interfaces; }
            const UTF8* getName() const { return name; }
            JavaClass* getSuper() const { return super; }

            bool isInterface() const { return jwasm::isInterface(access); }
            bool isClass() const { return jwasm::isClass(access); }
            bool isPrimitive() const { return jwasm::isPrimitive(access); }

            void setInterfaces(std::vector<JavaClass*> I) {
                interfaces = I;
            }
    };

    class JavaClass : public CommonJavaClass {
        public:
            JavaClass(JavaClassLoader* loader, const UTF8* name, ClassBytes* bytes);
            ~JavaClass();
            
            uint16_t virtualFields_count;
            std::vector<JavaField*> virtualFields;

            uint16_t staticFields_count;
            std::vector<JavaField*> staticFields;

            uint16_t virtualMethods_count;
            std::vector<JavaMethod*> virtualMethods;

            uint16_t staticMethods_count;
            std::vector<JavaMethod*> staticMethods;

            JavaConstantPool *constantPool;

            uint16_t attributes_count;
            std::vector<JavaAttribute*> attributes;

            uint16_t minJDKVersionMajor, minJDKVersionMinor, minJDKVersionBuild;

            JavaMethod* lookupMethod(const UTF8* name, const UTF8* type, bool isStatic, bool recurse, std::vector<JavaClass*> classes);
            JavaMethod* lookupInterfaceMethod(const UTF8* name, const UTF8* type);
            JavaField* lookupField(const UTF8* name, const UTF8* type, bool isStatic, bool recurse, std::vector<JavaClass*> classes);
            JavaAttribute* lookupAttribute(const UTF8* key);
            JavaConstantPool* getConstantPool() const { return constantPool; }

            void readParents(Reader& reader);
            void readFields(Reader& reader);
            void readMethods(Reader& reader);
            void readClass();

            static void getMinimalJDKVersion(uint16_t major, uint16_t minor, uint16_t& JDKMajor, 
                    uint16_t& JDKMinor, uint16_t& JDKBuild);
            bool isClassVersionSupported(uint16_t major, uint16_t minor);
            
            ClassBytes* bytes;
            JavaConstantPool* ctpInfo;
    };

    class JavaField {
        public:
            JavaField(JavaClass* cl, const UTF8* name, const UTF8* type, uint16_t access);
            ~JavaField();

            uint16_t access;
            const UTF8* name;
            const UTF8* type;

            uint16_t attributes_count;
            std::vector<JavaAttribute*> attributes;

            JavaClass* classDef;

            JavaAttribute* lookupAttribute(const UTF8* key);
    };

    class JavaMethod {
        public:
            enum Type {
                Static,
                Special,
                Interface,
                Virtual
            };
            
            JavaMethod(JavaClass* cl, const UTF8* name, const UTF8* type, uint16_t access);
            ~JavaMethod();

            uint16_t access;
            const UTF8* name;
            const UTF8* type;

            uint16_t attributes_count;
            std::vector<JavaAttribute*> attributes;

            JavaClass* classDef;

            JavaAttribute* lookupAttribute(const UTF8* key);
    };
}

#endif
