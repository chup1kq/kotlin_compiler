#include "Constructor.h"

#include "VarDeclarationList.h"

Constructor *Constructor::createPrimaryConstructor(
        ModifierMap* modifiers,
        ConstructorArgs* args,
        StmtListNode* stmts
) {
    Constructor *constructor = new Constructor();
    constructor->modifiers = modifiers;
    constructor->args = args;
    constructor->stmts = stmts;
    return constructor;
}

Constructor *Constructor::createSecondaryConstructor(
        ModifierMap* modifiers,
        VarDeclarationList *args,
        StmtListNode* stmts,
        ExprType prevConstructorType,
        VarDeclarationList* prevConstructorArgs
) {
    Constructor *constructor = new Constructor();
    constructor->modifiers = modifiers;
    constructor->args = ConstructorArgs::addVarDeclarationList(args);
    constructor->stmts = stmts;
    constructor->prevConstructorType = prevConstructorType;
    constructor->prevConstructorArgs = prevConstructorArgs;
    return constructor;
}

string Constructor::getDotLabel() const {
    return "Constructor" + (prevConstructorType ? ("from primary: " + exprTypeToString(prevConstructorType)) : "");
}

string Constructor::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, modifiers, "modifiers");
    addDotChild(dot, args, "args");
    addDotChild(dot, stmts, "statements");
    addDotChild(dot, prevConstructorArgs, "prevConstructorArgs");

    return dot;
}
