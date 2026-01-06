#ifndef KOTLIN_COMPILER_LOCALVARIABLETABLE_H
#define KOTLIN_COMPILER_LOCALVARIABLETABLE_H
#include "map"
#include "LocalVariableTableElement.h"

class LocalVariableTable {
public:
    int maxId = 0;
    std::map<std::string, LocalVariableTableElement*> items;
    int findOrAddLocalVar(std::string name,
        SemanticType * type,
        int isConst,
        int isInitialized);
};

#endif //KOTLIN_COMPILER_LOCALVARIABLETABLE_H