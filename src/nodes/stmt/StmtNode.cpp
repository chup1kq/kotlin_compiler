#include "StmtNode.h"

// создать while и do_while
StmtNode * StmtNode::createCycleNodeFromSingleStmt(StmtType type, ExprNode *cycleCond, StmtNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = type;
    node->cond = cycleCond;
    node->cycleSingleStmt = cycleStmt;
    return node;
}

// создать while и do_while
StmtNode * StmtNode::createCycleNodeFromBlockStmt(StmtType type, ExprNode *cycleCond, StmtListNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = type;
    node->cond = cycleCond;
    node->blockStmts = cycleStmt;
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

StmtNode * StmtNode::createEmptyStmt() {
    StmtNode* node = new StmtNode();
    node->type = EMPTY;
    return node;
}

// создать stmt на основе expr (когда добавляется перевод строки или ';')
StmtNode * StmtNode::createStmtFromExpr(ExprNode* expr) {
    StmtNode* node = new StmtNode();
    node->type = EXPRESSION;
    node->expr = expr;
    return node;
}

StmtNode *StmtNode::createVarOrValStmtNode(StmtType type, VarDeclaration *varDeclaration) {
    StmtNode* node = new StmtNode();
    node->type = type;
    node->varDeclaration = varDeclaration;
    return node;
}

StmtNode * StmtNode::createForNodeFromSingleStmt(string id, ExprNode *range, StmtNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = FOR;
    node->iteratorId = id;
    node->cond = range;
    node->cycleSingleStmt = cycleStmt;
    return node;
}

StmtNode * StmtNode::createForNodeFromBlockStmt(string id, ExprNode *range, StmtListNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = FOR;
    node->iteratorId = id;
    node->cond = range;
    node->blockStmts = cycleStmt;
    return node;
}

StmtNode * StmtNode::createReturnNode(ExprNode *expr) {
    StmtNode* node = new StmtNode();
    node->type = RETURN;
    node->expr = expr;
    return node;
}


