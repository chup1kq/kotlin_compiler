#ifndef KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#define KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#include "../baseTransformer/ASTTransformer.h"
#include "topLevel/KotlinFileNode.h"


class OperatorsTransformator : public ASTTransformer {
protected:
    void transformExpression(ExprNode* expr) override;
    void transformVarDeclaration(VarDeclaration* decl) override;
};



#endif //KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H