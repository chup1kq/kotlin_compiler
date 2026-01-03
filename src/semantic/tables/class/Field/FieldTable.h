#ifndef KOTLIN_COMPILER_FIELDTABLE_H
#define KOTLIN_COMPILER_FIELDTABLE_H
#include <map>
#include <string>
#include "../FieldTableElement.h"

class FieldTable {
public:
    std::map<std::string, FieldTableElement*> fields;
};

#endif //KOTLIN_COMPILER_FIELDTABLE_H