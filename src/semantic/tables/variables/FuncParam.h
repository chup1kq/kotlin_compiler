#ifndef KOTLIN_COMPILER_FUNCPARAM_H
#define KOTLIN_COMPILER_FUNCPARAM_H
#include "TypeTable.h"

class FuncParam {
public:
    std::string name;
    TypeTable * type;
    bool operator==(const FuncParam &) const;
    bool operator<(const FuncParam &) const;

    FuncParam(std::string name, TypeTable * type);
};

#endif //KOTLIN_COMPILER_FUNCPARAM_H