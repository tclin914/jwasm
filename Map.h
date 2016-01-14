#ifndef JWASM_MAP_H
#define JWASM_MAP_H

#include <map>
#include "UTF8.h"

namespace jwasm {

    class CommonJavaClass;

    class ClassMap {
        public: 
            ClassMap() {}
            ClassMap(std::map<const jwasm::UTF8*, CommonJavaClass*>* classmap) : map(*classmap) {}

            std::map<const jwasm::UTF8*, CommonJavaClass*> map;
            typedef std::map<const jwasm::UTF8*, CommonJavaClass*>::iterator iterator;
    };

}

#endif
