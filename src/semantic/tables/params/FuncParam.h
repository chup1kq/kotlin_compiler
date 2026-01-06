#ifndef KOTLIN_COMPILER_FUNCPARAM_H
#define KOTLIN_COMPILER_FUNCPARAM_H
#include "../type/SemanticType.h"

class FuncParam {
public:
    std::string name;
    SemanticType * type;
    // TODO добавить обработку параметров по умолчанию
    bool operator==(const FuncParam &) const;
    bool operator<(const FuncParam &) const;

    FuncParam(std::string name, SemanticType * type);
};

#endif //KOTLIN_COMPILER_FUNCPARAM_H