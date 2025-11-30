#include "FunNode.h"

FunNode * FunNode::createFunNode(TypeNode* returnType, ModifierMap* modifiers, string name, VarDeclarationList* args, StmtListNode* body) {
    FunNode* funNode = new FunNode();
    funNode->type = returnType;
    funNode->modifiers = modifiers;
    funNode->name = name;
    funNode->args = args;
    funNode->body = body;
    return funNode;
}

FunNode * FunNode::createFunNodeFromExpr(TypeNode* returnType, ModifierMap* modifiers, string name, VarDeclarationList* args, ExprNode* singleExpr) {
    FunNode* funNode = new FunNode();
    funNode->type = returnType;
    funNode->modifiers = modifiers;
    funNode->name = name;
    funNode->args = args;
    funNode->singleExpr = singleExpr;
    return funNode;
}
