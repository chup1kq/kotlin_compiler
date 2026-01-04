#ifndef KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#define KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#include "topLevel/KotlinFileNode.h"
#include "../baseTransformer/ASTTransformer.h"

class LiteralsTransformator : public ASTTransformer {
protected:
    void transformExpressionBody(ExprNode* expr) override;
    void transformVarDeclarationBody(VarDeclaration* decl) override;
};


#endif //KOTLIN_COMPILER_LITERALSTRASFORMATOR_H