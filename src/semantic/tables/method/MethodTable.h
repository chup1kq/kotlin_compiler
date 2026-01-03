#ifndef KOTLIN_COMPILER_METHODTABLE_H
#define KOTLIN_COMPILER_METHODTABLE_H
#include <map>
#include <string>

#include "MethodTableElement.h"

class MethodTable {
public:
    std::map<std::string, std::map<std::string, MethodTableElement *>> methods;
};

#endif //KOTLIN_COMPILER_METHODTABLE_H