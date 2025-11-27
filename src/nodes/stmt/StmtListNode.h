#ifndef KOTLIN_COMPILER_STMTLISTNODE_H
#define KOTLIN_COMPILER_STMTLISTNODE_H
#include <list>

#include "../Node.h"


class StmtListNode: public Node {
public:
    std::list<StmtNode*> *stmts;

    StmtListNode();
    StmtListNode(StmtNode *first);

    static StmtListNode* addStmtToList(StmtListNode *list, StmtNode *stmt);
    static StmtListNode* addExprToStmtList(StmtListNode *list, ExprNode *expr);
};


#endif //KOTLIN_COMPILER_STMTLISTNODE_H