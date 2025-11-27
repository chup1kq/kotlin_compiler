#ifndef KOTLIN_COMPILER_FUNNODE_H
#define KOTLIN_COMPILER_FUNNODE_H
#include "ExprListNode.h"
#include "Node.h"
#include "StmtListNode.h"
#include "types.h"


class FunNode : public Node {
public:
    Type type;
    ModifierType modifier;
    string name;
    ExprListNode args;
    StmtListNode body;

    static FunNode* createFunNode(Type type, ModifierType modifier, string name, ExprListNode args, StmtListNode body);
};


#endif //KOTLIN_COMPILER_FUNNODE_H