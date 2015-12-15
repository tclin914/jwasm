#ifndef JWASM_JAVA_ACCESS_H
#define JWASM_JAVA_ACCESS_H

namespace jwasm {

#define ACC_PUBLIC       0x0001
#define ACC_PRIVATE      0x0002
#define ACC_PROTECTED    0x0004
#define ACC_VIRTUAL      0x0000
#define ACC_STATIC       0x0008
#define ACC_FINAL        0x0010
#define ACC_SYNCHRONIZED 0x0020
#define ACC_SUPER        0x0020
#define ACC_VOLATILE     0x0040
#define ACC_TRANSIENT    0x0080
#define ACC_NATIVE       0x0100
#define ACC_INTERFACE    0x0200
#define ACC_ABSTRACT     0x0400
#define ACC_STRICT       0x0800
#define ACC_SYNTHETIC    0x1000
#define ACC_ENUM         0x4000


#define ACC_VERIFIER(name, flag)               \
    inline bool name(unsigned int param) {     \
        return (flag & param) != 0;            \
    }                                          \

ACC_VERIFIER(isStatic,    ACC_STATIC)
ACC_VERIFIER(isNative,    ACC_NATIVE)
ACC_VERIFIER(isInterface, ACC_INTERFACE)
ACC_VERIFIER(isSynchro,   ACC_SYNCHRONIZED)
ACC_VERIFIER(isPublic,    ACC_PUBLIC)
ACC_VERIFIER(isPrivate,   ACC_PRIVATE)
ACC_VERIFIER(isAbstract,  ACC_ABSTRACE)
ACC_VERIFIER(isProtected, ACC_PROTECTED)
ACC_VERIFIER(isFinal,     ACC_FINAL)
ACC_VERIFIER(isSuper,     ACC_SUPER)
ACC_VERIFIER(isSynthetic, ACC_SYNTHETIC)
ACC_VERIFIER(isEnum,      ACC_ENUM)

inline bool isVirtual(unsigned int param) {
    return !(ACC_STATIC & param);
}

#undef ACC_VERIFIER

}

#endif
