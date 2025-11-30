#ifndef KOTLIN_COMPILER_FUNNODE_H
#define KOTLIN_COMPILER_FUNNODE_H
#include "VarDeclarationList.h"
#include "../Node.h"
#include "modifier/ModifierMap.h"

class FunNode : public Node {
public:
    TypeNode* type;
    ModifierMap* modifiers;
    string name;
    VarDeclarationList* args;
    StmtListNode* body;
    ExprNode* singleExpr;

    static FunNode * createFunNode(TypeNode* returnType, ModifierMap* modifiers, string name, VarDeclarationList* args, StmtListNode* body);
    static FunNode * createFunNodeFromExpr(TypeNode* returnType, ModifierMap* modifiers, string name, VarDeclarationList* args, ExprNode* single);
};


#endif //KOTLIN_COMPILER_FUNNODE_H