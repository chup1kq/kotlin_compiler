#ifndef KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H
#define KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H
#include "topLevel/KotlinFileNode.h"


class JavaRTLTypesTransformator {
public:
    static void replaceTypesToJavaRTL(KotlinFileNode* root);

private:
    static void replaceToRTLInClasses(std::list<ClassNode*> classes);
    static void replaceToRTLInFunctions(std::list<FunNode*> functions);

    static void replaceToRTLInStatements(std::list<StmtNode*> stmts);

    static void replaceToRTLInStatement(StmtNode* stmt);
    static void replaceToRTLInVarDeclarations(std::list<VarDeclaration*> decls);

    static void replaceToRTLInTypeNode(TypeNode* typeNode);
};


#endif //KOTLIN_COMPILER_JAVARTLTYPESTRANSFORMATOR_H