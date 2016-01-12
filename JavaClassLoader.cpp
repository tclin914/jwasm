#include <map>
#include <cstdlib>

#include "JavaClassLoader.h"
#include "JavaClass.h"
#include "UTF8.h"
#include "Map.h"
#include "Reader.h"

using namespace jwasm;

JavaClassLoader::JavaClassLoader() {
    std::map<UTF8MapKey, const UTF8*> utf8map;
    hashUTF8 = new UTF8Map(&utf8map);
    classes = new ClassMap();
}

const UTF8* JavaClassLoader::asciizConstructUTF8(const char* asciiz) {
    return hashUTF8->lookupOrCreateAsciiz(asciiz);
}

JavaClass* JavaClassLoader::loadName(const UTF8* name, bool doResolve, bool doThrow) {
    JavaClass* cl = internalLoad(name, doResolve);
}

JavaClass* JavaClassLoader::internalLoad(const UTF8* name, bool doResolve) {
    ClassBytes* bytes = NULL;
    CommonJavaClass* cl = lookupClass(name);
    if (!cl) {
        bytes = openName(name);
        if (bytes != NULL) {
            cl = constructClass(name, bytes);                 
        }

    }

}

CommonJavaClass* JavaClassLoader::lookupClass(const UTF8* name) {
    CommonJavaClass* cl = classes->map.find(name)->second;
    return cl;
}

ClassBytes* JavaClassLoader::openName(const UTF8* name) {
    //TODO:class path
    char* asciiz = (char*)malloc(sizeof(char) * (name->size + 1));
    for (int32_t i = 0; i < name->size; ++i)
        asciiz[i] = name->elements[i];
    asciiz[name->size] = 0;
    ClassBytes* res = Reader::openFile(this, asciiz);    
}

JavaClass* JavaClassLoader::constructClass(const UTF8* name, ClassBytes* bytes) {
    JavaClass* res = NULL;

    return res;
}

