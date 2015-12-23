#include <map>

#include "JavaClassLoader.h"
#include "UTF8.h"

using namespace jwasm;

JavaClassLoader::JavaClassLoader() {
    std::map<UTF8MapKey, const UTF8*> utf8map;
    hashUTF8 = new UTF8Map(&utf8map);
}

const UTF8* JavaClassLoader::asciizConstructUTF8(const char* asciiz) {
    return hashUTF8->lookupOrCreateAsciiz(asciiz);
}

