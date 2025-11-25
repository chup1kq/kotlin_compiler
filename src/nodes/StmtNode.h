#ifndef KOTLIN_COMPILER_STMTNODE_H
#define KOTLIN_COMPILER_STMTNODE_H
#include <list>

#include "Node.h"
#include "types.h"


class StmtNode : public Node {
public:
    StmtType type;
    ExprNode *cond;
    StmtNode *cycleStmt;
    list<StmtNode *> blockStmts;

    static StmtNode* createCycleNode(StmtType  type, ExprNode* cycleCond, StmtNode* cycleStmt);
    static StmtNode* createBreakNode();
    static StmtNode* createContinueNode();
};


#endif //KOTLIN_COMPILER_STMTNODE_H