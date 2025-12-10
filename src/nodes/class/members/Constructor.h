#ifndef KOTLIN_COMPILER_CONSTRUCTOR_H
#define KOTLIN_COMPILER_CONSTRUCTOR_H
#include "ConstructorArgs.h"
#include "ModifierMap.h"
#include "Node.h"
#include "StmtListNode.h"

class VarDeclarationList;

class Constructor : public Node {
public:
    ModifierMap* modifiers;
    ConstructorArgs* args;
    StmtListNode* stmts;
    ExprType prevConstructorType;
    VarDeclarationList* prevConstructorArgs;

    static Constructor* createPrimaryConstructor(ModifierMap* modifiers, ConstructorArgs* args, StmtListNode* stmts);
    static Constructor* createSecondaryConstructor(ModifierMap* modifiers, VarDeclarationList* args, StmtListNode* stmts, ExprType prevConstructorType, VarDeclarationList* prevConstructorArgs);

    string toDot() const override;
    string getDotLabel() const override;
};


#endif //KOTLIN_COMPILER_CONSTRUCTOR_H