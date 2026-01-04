#ifndef KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#define KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H
#include "topLevel/KotlinFileNode.h"


class OperatorsTransformator {
public:
    static void replaceOperators(KotlinFileNode* root);

private:
    static void replaceOperatorsInClasses(std::list<ClassNode*> classes);
    static void replaceOperatorsInFunctions(std::list<FunNode*> functions);

    static void replaceOperatorsInStatements(std::list<StmtNode*> stmts);

    static void replaceOperatorsInStatement(StmtNode* stmt);
    static void replaceOperatorsInExpression(ExprNode* expr);
    static void replaceOperatorsInExpressions(ExprListNode* exprs);
    static void replaceOperatorsInVarDeclarations(std::list<VarDeclaration*> decls);
};


#endif //KOTLIN_COMPILER_OPERATORSTRANSFORMATOR_H