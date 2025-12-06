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

string FunNode::getDotLabel() const {
    return "FunNode_" + name;
}

string FunNode::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, type, "type");
    addDotChild(dot, modifiers, "modifiers");
    addDotChild(dot, args, "args");
    addDotChild(dot, body, "body");
    addDotChild(dot, singleExpr, "singleExpr");

    return dot;
}
