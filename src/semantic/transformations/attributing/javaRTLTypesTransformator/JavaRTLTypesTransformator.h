#ifndef KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H
#define KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H
#include "../../baseTransformer/ASTTransformer.h"
#include "topLevel/KotlinFileNode.h"


class JavaRTLTypesTransformator : public ASTTransformer {
protected:
    void transformExpressionBody(ExprNode* expr) override;
    void transformVarDeclarationBody(VarDeclaration* decl) override;

private:
    void replaceToRTLInTypeNode(TypeNode *typeNode);
};



#endif //KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H