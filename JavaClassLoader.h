#ifndef JWASM_JAVA_CLASS_LOADER_H
#define JWASM_JAVA_CLASS_LOADER_H

namespace jwasm {
    class UTF8;
    class UTF8Map;
    class CommonJavaClass;
    class JavaClass;
    class ClassMap;
    class ClassBytes;

    class JavaClassLoader {
        public:
            JavaClassLoader();
            ~JavaClassLoader();

            const UTF8* asciizConstructUTF8(const char* asciiz); 
            JavaClass* loadName(const UTF8* name, bool doResolve, bool doThrow);            
            JavaClass* internalLoad(const UTF8* name, bool doResolve);
            CommonJavaClass* lookupClass(const UTF8* name);
            ClassBytes* openName(const UTF8* name);
            JavaClass* constructClass(const UTF8* name, ClassBytes* bytes);
            CommonJavaClass* lookupClassOrArray(const UTF8* name);

            UTF8Map* hashUTF8;
            ClassMap* classes;
    };
}


#endif
