#ifndef JWASM_CLASSLOADER_H
#define JWASM_CLASSLOADER_H

namespace jwasm {
    class UTF8;
    class UTF8Map;

    class JavaClassLoader {
        public:
            JavaClassLoader();
            ~JavaClassLoader();

            const UTF8* asciizConstructUTF8(const char* asciiz); 

        private:
            UTF8Map* hashUTF8;
    };
}


#endif
