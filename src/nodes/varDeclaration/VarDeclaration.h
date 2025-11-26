#ifndef KOTLIN_COMPILER_VARDECLRATION_H
#define KOTLIN_COMPILER_VARDECLRATION_H
#include <string>

#include "../expr/ExprNode.h"


class VarDeclaration {
    std::string varId;
    TypeNode varType;
    ExprNode* defaultValue;

    VarDeclaration * createVarDeclaration(std::string varId, TypeNode varType, ExprNode* defaultValue);
};


#endif //KOTLIN_COMPILER_VARDECLRATION_H