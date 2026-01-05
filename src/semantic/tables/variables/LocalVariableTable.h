#ifndef KOTLIN_COMPILER_LOCALVARIABLETABLE_H
#define KOTLIN_COMPILER_LOCALVARIABLETABLE_H
#include "map"


class LocalVariableTableElement;

class LocalVariableTable {
public:
    int maxId = 0;
    std::map<std::string, LocalVariableTableElement> items;
    int findOrAddLocalVar(std::string name,
        TypeTable * type,
        int isConst,
        int isInit);
};

#endif //KOTLIN_COMPILER_LOCALVARIABLETABLE_H