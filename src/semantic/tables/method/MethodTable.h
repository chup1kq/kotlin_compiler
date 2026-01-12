#ifndef KOTLIN_COMPILER_METHODTABLE_H
#define KOTLIN_COMPILER_METHODTABLE_H
#include <map>
#include <string>

#include "MethodTableElement.h"

class MethodTable {
public:
    std::map<std::string, MethodTableElement*> methods;

    MethodTable();

    void addMethod(
        const std::string& methodName,
        const std::string& descriptor,
        MethodTableElement* element
    );

    bool contains(const std::string& methodName, const std::string& descriptor) const;
    MethodTableElement* getMethod(const std::string& methodName, const std::string& descriptor) const;
};

#endif //KOTLIN_COMPILER_METHODTABLE_H
