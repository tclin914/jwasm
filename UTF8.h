#ifndef JWASM_UTF8_H
#define JWASM_UTF8_H

#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdio>
#include <map>

namespace jwasm {

    class UTF8Map;

    class UTF8 {
        friend class UTF8Map;

        public:
            UTF8(int32_t n) { 
                size = n;
                elements = new uint16_t[n];
            }

            ~UTF8() {
                delete[] elements;
            }

            int32_t size;

            // uint16_t elements[1];
            uint16_t* elements;

            const UTF8* extract(UTF8Map* map, uint32_t start, uint32_t len) const;

            bool equals(const UTF8* other) const {
                if (other == this) return true;
                else if (size != other->size) return false;
                else return !memcmp(elements, other->elements, size * sizeof(uint16_t));
            }

            bool lessThan(const UTF8* other) const {
                if (size < other->size) return true;
                else if (size > other->size) return false;
                else return memcmp((const char*)elements, (const char*)other->elements, 
                        sizeof(uint16_t) * size) < 0;
            }

            static uint32_t readerHasher(const uint16_t* buf, int32_t size);

            uint32_t hash() const { return readerHasher(elements, size); }

            friend std::ostream& operator << (std::ostream&, const UTF8&);
            void dump() const __attribute__((noinline));
            int compare(const char*) const;
            std::string& toString(std::string& buffer) const;
    };

    struct UTF8MapKey {
        size_t length;
        const uint16_t* data;

        UTF8MapKey(const uint16_t* d, size_t l) {
            data = d;
            length = l;
        }

        bool operator < (const UTF8MapKey& other) const {
            if (length < other.length) return true;
            else if (length > other.length) return false;
            else return memcmp((const char*)data, (const char*)other.data, sizeof(uint16_t) * length);
        }
    };

    class UTF8Map {
        public:
            typedef std::map<UTF8MapKey, const UTF8*>::iterator iterator;
            std::map<UTF8MapKey, const UTF8*> map;

            const UTF8* lookupOrCreateAsciiz(const char* asciiz);
            const UTF8* lookupOrCreateReader(const uint16_t* buf, uint32_t size);
            const UTF8* lookupAsciiz(const char* asciiz);
            const UTF8* lookupReader(const uint16_t* buf, uint32_t size);

            UTF8Map(std::map<UTF8MapKey, const UTF8*>* m) : map(*m) {}
    
            //TODO:
            ~UTF8Map() {}
    };

    class UTF8Buffer {
        public:
            char* contents;

            UTF8Buffer(const UTF8* val) {
                contents = new char[val->size + 1];
                for (int i = 0; i < val->size; i++) {
                    contents[i] = (char)val->elements[i];
                }
                contents[val->size] = 0;
            }
            
            char* cString() const {
                return contents;
            }

            ~UTF8Buffer() {
                delete[] contents;
            }
    };
}

#endif
