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
    return createFunNode(
        returnType,
        modifiers,
        name,
        args,
        StmtListNode::addExprToStmtList(nullptr, singleExpr)
    );
}

string FunNode::getDotLabel() const {
    return "fun " + name;
}

string FunNode::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, type, "type");
    addDotChild(dot, modifiers, "modifiers");
    addDotChild(dot, args, "args");
    addDotChild(dot, body, "body");

    return dot;
}
