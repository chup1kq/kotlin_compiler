#ifndef KOTLIN_COMPILER_VARDECLRATION_H
#define KOTLIN_COMPILER_VARDECLRATION_H
#include <string>

#include "../expr/ExprNode.h"


class VarDeclaration : public Node  {
public:
    std::string varId;
    TypeNode* varType;
    ExprNode* defaultValue;

    static VarDeclaration * createVarDeclaration(std::string varId, TypeNode* varType, ExprNode* defaultValue);

    string toDot() const override;
    string getDotLabel() const override;
};


#endif //KOTLIN_COMPILER_VARDECLRATION_H