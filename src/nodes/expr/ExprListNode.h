#ifndef KOTLIN_COMPILER_EXPRLISTNODE_H
#define KOTLIN_COMPILER_EXPRLISTNODE_H
#include <list>

#include "../Node.h"


class ExprListNode: public Node {
public:
    std::list<ExprNode*> *exprs;

    ExprListNode();
    ExprListNode(ExprNode* expr);

    static ExprListNode* addExprToList(ExprListNode *list, ExprNode *expr);
};


#endif //KOTLIN_COMPILER_EXPRLISTNODE_H