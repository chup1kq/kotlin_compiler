#include "Constructor.h"

Constructor *Constructor::createPrimaryConstructor(
        ModifierMap* modifiers,
        VarDeclarationList *args,
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
    constructor->args = args;
    constructor->stmts = stmts;
    constructor->prevConstructorType = prevConstructorType;
    constructor->prevConstructorArgs = prevConstructorArgs;
    return constructor;
}

