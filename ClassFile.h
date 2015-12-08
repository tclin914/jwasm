#ifndef JWASM_CLASSFILE_H
#define JWASM_CLASSFILE_H

#include <stdint.h>
#include <vector>
#include "ConstantPool.h"

namespace jwasm {
    
    class JavaAttribute {
        public:
            uint16_t attribute_name_index;
            uint32_t attribute_length;
            unsigned char* start;
    };

    class JavaClassMember {
        public: 
            uint16_t access_flags;
            uint16_t name_index;
            uint16_t descriptor_index;
            uint16_t attributes_count;
            std::vector<JavaAttribute> attributes;
    };

    class JavaField : public JavaClassMember {

    };

    class JavaMethod : public JavaClassMember {
    
    };
    
    class JavaClass {
        public:
            uint32_t magic_number;
            uint16_t minor_version;
            uint16_t max_version;
            uint16_t constant_pool_count;
            std::vector<CP_Info> constant_pool;
            uint16_t access_flags;
            uint16_t this_class;
            uint16_t super_class;
            uint16_t interfaces_count;
            std::vector<uint16_t> interfaces;
            uint16_t fields_count;
            std::vector<JavaField> fields;
            uint16_t methods_count;
            std::vector<JavaMethod> methods;
            uint16_t attributes_count;
            std::vector<JavaAttribute> attributes;
    };
}



#endif
