#ifndef KOTLIN_COMPILER_BASETYPESATTRIBUTE_H
#define KOTLIN_COMPILER_BASETYPESATTRIBUTE_H
#include "topLevel/KotlinFileNode.h"
#include "../../baseTransformer/ASTTransformer.h"


class BaseTypesAttributing : public ASTTransformer {
protected:
    void transformExpressionBody(ExprNode* expr) override;
    void transformVarDeclarationBody(VarDeclaration* decl) override;
};


#endif //KOTLIN_COMPILER_BASETYPESATTRIBUTE_H