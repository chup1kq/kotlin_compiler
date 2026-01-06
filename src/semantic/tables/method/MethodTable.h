#ifndef KOTLIN_COMPILER_METHODTABLE_H
#define KOTLIN_COMPILER_METHODTABLE_H
#include <map>
#include <string>

#include "MethodTableElement.h"

class MethodTable {
public:
    // Вложенная map для поддержки перегрузки
    std::map<std::string, std::map<std::string, MethodTableElement *>> methods;

    MethodTable();

    void addMethod(
        const std::string& methodName,
        const std::string& descriptor,
        MethodTableElement* element
    );

    // TODO для поддержки перегрузок
    // vector<MethodTableElement*> findCandidates(const std::string& name);
};

#endif //KOTLIN_COMPILER_METHODTABLE_H