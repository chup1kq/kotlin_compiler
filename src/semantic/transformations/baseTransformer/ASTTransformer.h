#ifndef KOTLIN_COMPILER_ASTTRANSFORMER_H
#define KOTLIN_COMPILER_ASTTRANSFORMER_H

#include "topLevel/KotlinFileNode.h"

class ASTTransformer {
public:
    virtual ~ASTTransformer() = default;

    void transform(KotlinFileNode* root);

protected:
    virtual void transformExpressionBody(ExprNode* expr) = 0;
    virtual void transformVarDeclarationBody(VarDeclaration* decl) = 0;


    void transformClasses(std::list<ClassNode*> classes);
    void transformFunctions(std::list<FunNode*> functions);

    void transformConstructors(std::list<Constructor*> constructors);
    void transformConstructor(Constructor* constructor);

    void transformStatements(std::list<StmtNode*> stmts);
    void transformStatement(StmtNode* stmt);

    void transformExpressions(ExprListNode* exprs);
    void transformExpression(ExprNode* expr);

    void transformVarDeclarations(std::list<VarDeclaration*> decls);
    void transformVarDeclaration(VarDeclaration* decl);

    virtual void transformTypeNode(TypeNode* typeNode);
};

#endif
