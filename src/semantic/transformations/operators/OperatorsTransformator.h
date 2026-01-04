#ifndef KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#define KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#include "../baseTransformer/ASTTransformer.h"
#include "topLevel/KotlinFileNode.h"


class OperatorsTransformator : public ASTTransformer {
protected:
    void transformExpressionBody(ExprNode* expr) override;
    void transformVarDeclarationBody(VarDeclaration* decl) override;
};



#endif //KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H