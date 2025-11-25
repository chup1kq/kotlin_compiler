#include "StmtNode.h"

StmtNode * StmtNode::createCycleNode(StmtType type, ExprNode *cycleCond, StmtNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = type;
    node->cond = cycleCond;
    node->cycleStmt = cycleStmt;
    return node;
}

StmtNode * StmtNode::createBreakNode() {
    StmtNode* node = new StmtNode();
    node->type = BREAK;
    return node;
}

StmtNode * StmtNode::createContinueNode() {
    StmtNode* node = new StmtNode();
    node->type = CONTINUE;
    return node;
}
