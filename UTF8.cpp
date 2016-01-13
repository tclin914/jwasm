#include <cstdlib>
#include "UTF8.h"

namespace jwasm {

const UTF8* UTF8::extract(UTF8Map* map, uint32_t start, uint32_t end) const {
    uint32_t len = end - start;
    uint16_t* buf = (uint16_t*)malloc(sizeof(uint16_t) * len);

    for (uint32_t i = 0; i < len; i++) {
        buf[i] = elements[i + start];
    }

    return map->lookupOrCreateReader(buf, len);
}

uint32_t UTF8::readerHasher(const uint16_t* buf, int32_t size) {
    uint32_t r0 = 0, r1 = 0;
    for (int32_t i = 0; i < size; i++) {
        uint16_t c = buf[i];
        r0 += c;
        r1 ^= c;
    }
    return (r1 & 255) + ((r0 & 255) << 8);
}

int UTF8::compare(const char* s) const {
    int len = strlen(s);
    int diff = size - len;
    if (diff != 0) return diff;

    for (uint32_t i = 0; (i < size) && (diff == 0); ++i)
        diff = (char)(elements[i]) - s[i];
    return diff;
}

std::string& UTF8::toString(std::string& buffer) const {
    buffer.resize(size);

    for (ssize_t i = 0; i < size; ++i)
        buffer[i] = (std::string::value_type)(elements[i]);
    return buffer;
}

std::ostream& operator << (std::ostream& os, const UTF8& utf8) {
    for (ssize_t i = 0; i < utf8.size; ++i) {
        os << (std::string::value_type)(utf8.elements[i]);
    }
    return os;
}

void UTF8::dump() const {
    std::cerr << (const void*)this << "=\"" << *this << '\"' << std::endl;
}

const UTF8* UTF8Map::lookupOrCreateAsciiz(const char* asciiz) {
    int32_t size = strlen(asciiz);
    uint16_t* data = (uint16_t*)malloc(sizeof(uint16_t) * size);
    for (int i = 0; i < size; i++) {
        data[i] = asciiz[i];
    }
    return lookupOrCreateReader(data, size); 
}

const UTF8* UTF8Map::lookupOrCreateReader(const uint16_t* buf, uint32_t len) {
    int32_t size = (int32_t)len;
    UTF8MapKey key(buf, size);

    std::map<UTF8MapKey, const UTF8*>::iterator it;
    it = map.find(key);

    const UTF8* res = it->second;
    if (it == map.end()) {
        UTF8* tmp = new UTF8(size);
        memcpy(tmp->elements, buf, sizeof(uint16_t) * len);
        res = (const UTF8*)tmp;
        key.data = res->elements;
        map[key] = res;
    }
    return res;
}

const UTF8* UTF8Map::lookupAsciiz(const char* asciiz) {
    int32_t size = strlen(asciiz);
    uint16_t* data = (uint16_t*)malloc(sizeof(uint16_t) * size);
    for (int i = 0; i < size; i++) {
        data[i] = asciiz[i];
    }
    return lookupReader(data, size);
}

const UTF8* UTF8Map::lookupReader(const uint16_t* buf, uint32_t len) {
    int32_t size = (int32_t)len;
    UTF8MapKey key(buf, size);
    const UTF8* res = map.find(key)->second;
    return res;
}

}
