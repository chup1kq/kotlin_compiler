#ifndef KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#define KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#include "topLevel/KotlinFileNode.h"


class LiteralsTransformator {
public:
    static void replaceLiteralsToObjects(KotlinFileNode* root);

private:
    static void replaceLiteralsToObjectsInKotlinElement(KotlinFileNode * root);
    static void replaceLiteralsToObjectsInKotlinElementList(KotlinElementList * root);

    static void replaceLiteralsToObjectsInExpression(ExprNode * root);
    static void replaceLiteralsToObjectsInExpressionList(ExprListNode * root);

    static void replaceLiteralsToObjectsInStatement(StmtNode * root);
    static void replaceLiteralsToObjectsInStatementList(StmtListNode * root);

    static void replaceLiteralsToObjectsInFunction(FunNode * root);
    static void replaceLiteralsToObjectsInClass(ClassNode * root);
    static void replaceLiteralsToObjectsInClassBody(ClassBodyNode * root);
    static void replaceLiteralsToObjectsInInheritance(Inheritance * root);
    static void replaceLiteralsToObjectsInConstructor(Constructor * root);
    static void replaceLiteralsToObjectsInConstructorArgs(ConstructorArgs * root);
    static void replaceLiteralsToObjectsInVarDeclarationList(VarDeclarationList * root);
    static void replaceLiteralsToObjectsInVarDeclaration(VarDeclaration * root);


};


#endif //KOTLIN_COMPILER_LITERALSTRASFORMATOR_H