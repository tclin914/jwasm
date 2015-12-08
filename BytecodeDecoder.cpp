#include <stdio.h>
#include <stdlib.h>

#include "BytecodeDecoder.h"
#include "Reader.h"
#include "ConstantPool.h"
#include "ClassFile.h"

#define MAGIC_NUMBER 0xcafebabe
#define debug

using namespace jwasm;

BytecodeDecoder::BytecodeDecoder(Reader& reader, JavaClass& javaClass) : reader(reader), javaClass(javaClass) {

    checkMagicNumber();
    readVersion();
    readConstantPool();
    readAccess();
    readThis();
    readSuper();

    readInterfaces();
    readFields();
    readMethods();
    readAttributes();
}

void BytecodeDecoder::checkMagicNumber() {
    uint32_t magic_number = reader.readU4();
    if (magic_number != MAGIC_NUMBER) {
        fprintf(stderr, "Magic number is not correct\n");
        exit(1);
    }
    javaClass.magic_number = magic_number;
}

void BytecodeDecoder::readVersion() {
    javaClass.minor_version = reader.readU2();
    javaClass.max_version = reader.readU2();
}

void BytecodeDecoder::readConstantPool() {
    unsigned short constant_pool_count;
    constant_pool_count = reader.readU2();
    javaClass.constant_pool_count = constant_pool_count;
    printf("constant_pool_count = %hd\n", constant_pool_count);
    int count = 0;
    while (count < constant_pool_count - 1) {
        char tag = reader.readU1();
        switch (tag) {
            case CONSTANT_CLASS:
                CONSTANT_Class_info class_info;
                class_info.tag = tag;
                class_info.name_index = reader.readU2();
                javaClass.constant_pool.push_back(class_info);
                break;
            case CONSTANT_FIELDREF:
                CONSTANT_Fieldref_info fieldref_info;
                fieldref_info.tag = tag;
                fieldref_info.class_index = reader.readU2();
                fieldref_info.name_and_type_index = reader.readU2();
                javaClass.constant_pool.push_back(fieldref_info);
                break;
            case CONSTANT_METHODREF:
                CONSTANT_Methodref_info methodref_info;
                methodref_info.tag = tag;
                methodref_info.class_index = reader.readU2();
                methodref_info.name_and_type_index = reader.readU2();
                javaClass.constant_pool.push_back(methodref_info);
                break;
            case CONSTANT_INTERFACEMETHODREF:
                CONSTANT_InterfaceMethodref_info interfacemethodref_info;
                interfacemethodref_info.tag = tag;
                interfacemethodref_info.class_index = reader.readU2();
                interfacemethodref_info.name_and_type_index = reader.readU2();
                javaClass.constant_pool.push_back(interfacemethodref_info);
                break;
            case CONSTANT_STRING:
                CONSTANT_String_info string_info;
                string_info.tag = tag;
                string_info.string_index = reader.readU2();
                javaClass.constant_pool.push_back(string_info);
                break;
            case CONSTANT_INTEGER:
                CONSTANT_Integer_info integer_info;
                integer_info.tag = tag;
                integer_info.bytes = reader.readU4();
                javaClass.constant_pool.push_back(integer_info);
                break;
            case CONSTANT_FLOAT:
                CONSTANT_Float_info float_info;
                float_info.tag = tag;
                float_info.bytes = reader.readU4();
                javaClass.constant_pool.push_back(float_info);
                break;
            case CONSTANT_LONG:
                CONSTANT_Long_info long_info;
                long_info.tag = tag;
                long_info.high_bytes = reader.readU4();
                long_info.low_bytes = reader.readU4();
                javaClass.constant_pool.push_back(long_info);
                break;
            case CONSTANT_DOUBLE:
                CONSTANT_Double_info double_info;
                double_info.tag = tag;
                double_info.high_bytes = reader.readU4();
                javaClass.constant_pool.push_back(double_info);
                break;
            case CONSTANT_NAMEANDTYPE:
                CONSTANT_NameAndType_info nameandtype_info;
                nameandtype_info.tag = tag;
                nameandtype_info.name_index = reader.readU2();
                nameandtype_info.descriptor_index = reader.readU2();
                javaClass.constant_pool.push_back(nameandtype_info);
                break;
            case CONSTANT_UTF8:
                CONSTANT_Utf8_info utf8_info;
                utf8_info.tag = tag;
                utf8_info.length = reader.readU2();
                utf8_info.bytes = reader.getCursor();
                reader.move(utf8_info.length);
                break;
            case CONSTANT_METHODHANDLE:
                CONSTANT_MethodHandle_info methodhandle_info;
                methodhandle_info.tag = tag;
                methodhandle_info.reference_kind = reader.readU1();
                methodhandle_info.reference_index = reader.readU2();
                javaClass.constant_pool.push_back(methodhandle_info);
                break;
            case CONSTANT_METHODTYPE:
                CONSTANT_MethodType_info methodtype_info;
                methodtype_info.tag = tag;
                methodtype_info.descriptor_index = reader.readU2();
                javaClass.constant_pool.push_back(methodtype_info);
                break;
            case CONSTANT_INVOKEDYNAMIC:
                CONSTANT_InvokeDynamic_info invokedynamic_info;
                invokedynamic_info.tag = tag;
                invokedynamic_info.bootstrap_method_attr_index = reader.readU2();
                invokedynamic_info.name_and_type_index = reader.readU2();
                javaClass.constant_pool.push_back(invokedynamic_info);
                break;
            default:           
                break;
        }
        ++count;
    }
}

void BytecodeDecoder::readAccess() {
    javaClass.access_flags = reader.readU2();
}

void BytecodeDecoder::readThis() {
    javaClass.this_class = reader.readU2();
}

void BytecodeDecoder::readSuper() {
    javaClass.super_class = reader.readU2();
}

void BytecodeDecoder::readInterfaces() {
    unsigned short interfaces_count = reader.readU2();
    javaClass.interfaces_count = interfaces_count;
    printf("interfaces_count = %d\n", interfaces_count);
    for (unsigned short i = 0; i < interfaces_count; i++) {
        uint16_t index = reader.readU2();
        javaClass.interfaces.push_back(index);
    }
}

void BytecodeDecoder::resolveAttrubutes(std::vector<JavaAttribute>& attributes, Reader& reader, 
        uint16_t& count) {
    unsigned short attributes_count = reader.readU2();
    count = attributes_count;
    printf("attributes_count = %d\n", attributes_count);
    for (unsigned short i = 0; i < attributes_count; i++) {
        JavaAttribute attribute;
        attribute.attribute_name_index = reader.readU2();
        attribute.attribute_length = reader.readU4();
        attribute.start = reader.getCursor();
        reader.move(attribute.attribute_length);
        attributes.push_back(attribute);
    }
}

void BytecodeDecoder::readFields() {
    unsigned short fields_count = reader.readU2();
    printf("fields_count = %d\n", fields_count);
    for (unsigned short i = 0; i < fields_count; i++) {
        JavaField field;
        field.access_flags = reader.readU2();
        field.name_index = reader.readU2();
        field.descriptor_index = reader.readU2();
        resolveAttrubutes(field.attributes, reader, javaClass.fields_count);        
    }
}

void BytecodeDecoder::readMethods() {
    unsigned short methods_count = reader.readU2();
    printf("methods_count = %d\n", methods_count);
    for (unsigned short i = 0; i < methods_count; i++) {
        JavaMethod method;
        method.access_flags = reader.readU2();
        method.name_index = reader.readU2();
        method.descriptor_index = reader.readU2();
        resolveAttrubutes(method.attributes, reader, javaClass.methods_count);
    }
}

void BytecodeDecoder::readAttributes() {
    resolveAttrubutes(javaClass.attributes, reader, javaClass.attributes_count);
}
