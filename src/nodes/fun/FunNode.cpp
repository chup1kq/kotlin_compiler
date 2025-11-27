#include "FunNode.h"

FunNode * FunNode::createFunNode(Type type, ModifierType modifier, string name, ExprListNode args, StmtListNode body) {
    FunNode* funNode = new FunNode();
    funNode->type = type;
    funNode->modifier = modifier;
    funNode->name = name;
    funNode->args = args;
    funNode->body = body;
    return funNode;
}
