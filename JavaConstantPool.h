#ifndef JWASM_JAVA_CONSTANT_POOL
#define JWASM_JAVA_CONSTANT_POOL

#include "UTF8.h"

#define CONSTANT_UTF8               0x01
#define CONSTANT_INTEGER            0x03
#define CONSTANT_FLOAT              0x04
#define CONSTANT_LONG               0x05
#define CONSTANT_DOUBLE             0x06
#define CONSTANT_CLASS              0x07
#define CONSTANT_STRING             0x08
#define CONSTANT_FIELDREF           0x09
#define CONSTANT_METHODREF          0x0a
#define CONSTANT_INTERFACEMETHODREF 0x0b
#define CONSTANT_NAMEANDTYPE        0x0c
#define CONSTANT_METHODHANDLE       0x0f
#define CONSTANT_METHODTYPE         0x10
#define CONSTANT_INVOKEDYNAMIC      0x12

namespace jwasm {
    class JavaClass;
    class CommonJavaClass;
    class JavaField;
    class JavaMethod;
    class JavaString;
    class Reader;
    class Signdef;
    class Typedef;

    class JavaConstantPool {
        public:
            JavaClass* classDef;
            
            uint32_t ctpSize;
            
            uint8_t* ctpType;
           
            int32_t* ctpDef;
            
            void** ctpRes;

            static uint32_t CtpReaderClass(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderInteger(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderFloat(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderUTF8(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderNameAndType(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderFieldref(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderString(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderMethodref(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderInterfaceMethodref(JavaConstantPool* ctp, Reader& reader, uint32_t index);             
            static uint32_t CtpReaderLong(JavaConstantPool* ctp, Reader& reader, uint32_t index);
            static uint32_t CtpReaderDouble(JavaConstantPool* ctp, Reader& reader, uint32_t index);

            static const uint32_t ConstantUTF8;
            static const uint32_t ConstantInteger;
            static const uint32_t ConstantFloat;
            static const uint32_t ConstantLong;
            static const uint32_t ConstantDouble;
            static const uint32_t ConstantClass;
            static const uint32_t ConstantString;
            static const uint32_t ConstantFieldref;
            static const uint32_t ConstantMethodref;
            static const uint32_t ConstantInterfaceMethodref;
            static const uint32_t ConstantNameAndType;

            typedef uint32_t (*ctpReader)(JavaConstantPool*, Reader&, uint32_t);
            static ctpReader funcsReader[16];

            bool isAStaticCall(uint32_t index) {
                return (ctpType[index] & 0x80) != 0;
            }

            void markAsStaticCall(uint32_t index) {
                ctpType[index] |= 0x80;
            }

            uint8_t typeAt(uint32_t index) {
                return ctpType[index] & 0x7f;
            }

            const UTF8* UTF8At(uint32_t entry);
            const UTF8* UTF8AtForString(uint32_t entry) {
                return UTF8At(ctpDef[entry]);
            }

            float FloatAt(uint32_t entry);
            int32_t IntegerAt(uint32_t entry);
            int64_t LongAt(uint32_t entry);
            double DoubleAt(uint32_t entry);

            CommonJavaClass* isClassLoaded(uint32_t index);

            const UTF8* resolveClassName(uint32_t index);
            Typedef* resolveNameAndType(uint32_t index);
            Signdef* resolveNameAndSign(uint32_t index);
            Signdef* infoOfInterfaceOrVirtualMethod(uint32_t index, const UTF8*& name);
            JavaMethod* infoOfStaticOrSpecialMethod(uint32_t index, uint32_t access, Signdef* sign);
            void nameOfStaticOrSpecialMethod(uint32_t index, const UTF8*& cl, 
                    const UTF8*& name, Signdef*& sign);
            uint32_t getClassIndexFromMethod(uint32_t index);
            CommonJavaClass* getMethodClassIfLoaded(uint32_t index);
            Typedef* infoOfField(uint32_t index);
            void infoOfMethod(uint32_t index, uint32_t access, CommonJavaClass*& cl, JavaMethod*& meth);
            JavaField* lookupField(uint32_t index, bool stat);
            JavaString& resolveString(const UTF8* utf8, uint16_t index);
            void resolveMethod(uint32_t index, CommonJavaClass*& cl, const UTF8*& utf8, Signdef*& sign);
            void resolveField(uint32_t index, CommonJavaClass*& cl, const UTF8*& utf8, Typedef*& sign);
            CommonJavaClass* loadClass(uint32_t index, bool resolve = true);

            JavaConstantPool(JavaClass* cl, Reader& reader, uint32_t ctpSize);
            ~JavaConstantPool();
    };

}

#endif
