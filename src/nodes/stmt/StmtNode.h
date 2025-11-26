#ifndef KOTLIN_COMPILER_STMTNODE_H
#define KOTLIN_COMPILER_STMTNODE_H
#include <list>

#include "../Node.h"
#include "StmtListNode.h"
#include "../types.h"
#include "../varDeclaration/VarDeclaration.h"


class StmtNode : public Node {
public:
    StmtType type;
    ExprNode *cond;
    StmtNode *cycleSingleStmt;
    StmtListNode *blockStmts;
    ExprNode *expr;
    VarDeclaration *varDeclaration;
    string iteratorId;

    static StmtNode* createCycleNodeFromSingleStmt(StmtType type, ExprNode* cycleCond, StmtNode* cycleStmt);
    static StmtNode* createCycleNodeFromBlockStmt(StmtType type, ExprNode* cycleCond, StmtListNode* cycleStmt);
    static StmtNode* createForNodeFromSingleStmt(string id, ExprNode* range, StmtNode* cycleStmt);
    static StmtNode* createForNodeFromBlockStmt(string id, ExprNode* range, StmtListNode* cycleStmt);
    static StmtNode* createBreakNode();
    static StmtNode* createContinueNode();
    static StmtNode* createEmptyStmt();
    static StmtNode* createStmtFromExpr(ExprNode* expr);
    static StmtNode* createVarOrValStmtNode(StmtType type, VarDeclaration* varDeclaration);
    static StmtNode * createReturnNode(ExprNode* expr);

};


#endif //KOTLIN_COMPILER_STMTNODE_H