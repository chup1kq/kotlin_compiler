#ifndef KOTLIN_COMPILER_FIELDTABLE_H
#define KOTLIN_COMPILER_FIELDTABLE_H
#include <map>
#include <string>
#include "FieldTableElement.h"

class FieldTable {
public:
    std::map<std::string, FieldTableElement*> fields;

    FieldTable();

    void addField(
        const std::string& name,
        const std::string& descriptor,
        FieldTableElement* element
    );

    bool contains(const std::string& filedName);
};

#endif //KOTLIN_COMPILER_FIELDTABLE_H