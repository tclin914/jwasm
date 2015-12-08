#ifndef JWASM_CONSTANT_POOL
#define JWASM_CONSTANT_POOL

#include "stdint.h"

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

    class CP_Info {
        public:
            uint8_t tag;
    };

    class CONSTANT_Class_info : public CP_Info {
        public:
            uint16_t name_index;
    };

    class CONSTANT_Fieldref_info : public CP_Info {
        public:
            uint16_t class_index;
            uint16_t name_and_type_index;
    };

    class CONSTANT_Methodref_info : public CP_Info {
        public:
            uint16_t class_index;
            uint16_t name_and_type_index;
    };
    
    class CONSTANT_InterfaceMethodref_info : public CP_Info {
        public:
            uint16_t class_index;
            uint16_t name_and_type_index;
    };
 
    class CONSTANT_String_info : public CP_Info {
        public:
            uint16_t string_index;
    };

    class CONSTANT_Integer_info : public CP_Info {
        public:
            uint32_t bytes;
    };

    class CONSTANT_Float_info : public CP_Info {
        public:
            uint32_t bytes;
    };

    class CONSTANT_Long_info : public CP_Info {
        public:
            uint32_t high_bytes;
            uint32_t low_bytes;
    };

    class CONSTANT_Double_info : public CP_Info {
        public:
            uint32_t high_bytes;
            uint32_t low_bytes; 
    };

    class CONSTANT_NameAndType_info : public CP_Info {
        public:
            uint16_t name_index;
            uint16_t descriptor_index;
    };

    class CONSTANT_Utf8_info : public CP_Info {
        public:
            uint16_t length;
            unsigned char* bytes;
    };

    class CONSTANT_MethodHandle_info : public CP_Info {
        public:
            uint8_t reference_kind;
            uint16_t reference_index;
    };

    class CONSTANT_MethodType_info : public CP_Info {
        public:
            uint16_t descriptor_index;
    };

    class CONSTANT_InvokeDynamic_info : public CP_Info {
        public:
            uint16_t bootstrap_method_attr_index;
            uint16_t name_and_type_index;
    };
}

#endif
