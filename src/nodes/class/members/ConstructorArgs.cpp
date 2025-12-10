#include "ConstructorArgs.h"

#include "StmtListNode.h"

ConstructorArgs *ConstructorArgs::addMember(ConstructorArgs *args, VarDeclaration *declaration) {
    if (!args) {
        args = new ConstructorArgs();
        args->stmtArgs = new StmtListNode();
        args->simpleArgs = new VarDeclarationList(nullptr);
    }

    args->simpleArgs->addVarDeclarationToList(args->simpleArgs, declaration);
    return args;
}

ConstructorArgs* ConstructorArgs::addMember(ConstructorArgs* args, StmtNode* stmt) {
    if (!args) {
        args = new ConstructorArgs();
        args->simpleArgs = new VarDeclarationList(nullptr);
        args->stmtArgs = new StmtListNode();
    }

    args->stmtArgs->addStmtToList(args->stmtArgs, stmt);
    return args;
}

ConstructorArgs* ConstructorArgs::addVarDeclarationList(VarDeclarationList *declarations) {
    ConstructorArgs* args = new ConstructorArgs();
    args->simpleArgs = new VarDeclarationList(nullptr);
    args->stmtArgs = new StmtListNode();
    args->simpleArgs = declarations;
    return args;
}

string ConstructorArgs::getDotLabel() const {
    return "ConstructorArgs";
}

string ConstructorArgs::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, simpleArgs, "simpleArgs");
    addDotChild(dot, stmtArgs, "stmtArgs");

    return dot;
}
