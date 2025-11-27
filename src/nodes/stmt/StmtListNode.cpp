#include "StmtListNode.h"

#include "StmtNode.h"

StmtListNode::StmtListNode(StmtNode *first) {
    stmts = new std::list<StmtNode*>();
    stmts->push_back(first);
}

StmtListNode * StmtListNode::addStmtToList(StmtListNode *list, StmtNode *stmt) {
    if (list == nullptr) {
        return new StmtListNode(stmt);
    }

    list->stmts->push_back(stmt);
    return list;
}

StmtListNode * StmtListNode::addExprToStmtList(StmtListNode *list, ExprNode *expr) {
    StmtNode* exprToStmt = StmtNode::createStmtFromExpr(expr);

    if (list == nullptr) {
        return new StmtListNode(exprToStmt);
    }

    list->stmts->push_back(exprToStmt);
    return list;
}

