#include "StmtNode.h"

#include "VarDeclarationList.h"

StmtNode::StmtNode()
    : type(_EMPTY),
      cond(nullptr),
      cycleSingleStmt(nullptr),
      blockStmts(nullptr),
      expr(nullptr),
      varDeclaration(nullptr),
      forIterator(nullptr),
      forIteratorList(nullptr),
      varModifiers(nullptr),
      trueStmtList(nullptr),
      falseStmtList(nullptr)
{}

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
    node->type = _BREAK;
    return node;
}

StmtNode * StmtNode::createContinueNode() {
    StmtNode* node = new StmtNode();
    node->type = _CONTINUE;
    return node;
}

StmtNode * StmtNode::createEmptyStmt() {
    StmtNode* node = new StmtNode();
    node->type = _EMPTY;
    return node;
}

// создать stmt на основе expr (когда добавляется перевод строки или ';')
StmtNode * StmtNode::createStmtFromExpr(ExprNode* expr) {
    StmtNode* node = new StmtNode();
    node->type = _EXPRESSION;
    node->expr = expr;
    return node;
}

StmtNode *StmtNode::createVarOrValStmtNode(StmtType type, VarDeclaration *varDeclaration) {
    StmtNode* node = new StmtNode();
    node->type = type;
    node->varDeclaration = varDeclaration;
    return node;
}

StmtNode *StmtNode::createVarOrValStmtNode(ModifierMap* modifiers, StmtType type, VarDeclaration *varDeclaration) {
    StmtNode* node = new StmtNode();
    node->varModifiers = modifiers;
    node->type = type;
    node->varDeclaration = varDeclaration;
    return node;
}

StmtNode * StmtNode::createForNodeFromSingleStmt(VarDeclaration* iterator, ExprNode *range, StmtNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = _FOR;
    node->forIterator = iterator;
    node->cond = range;
    node->cycleSingleStmt = cycleStmt;
    return node;
}

StmtNode * StmtNode::createForNodeFromBlockStmt(VarDeclaration* iterator, ExprNode *range, StmtListNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = _FOR;
    node->forIterator = iterator;
    node->cond = range;
    node->blockStmts = cycleStmt;
    return node;
}

StmtNode * StmtNode::createForNodeFromSingleStmtWithSeveralId(VarDeclarationList *list, ExprNode *range, StmtNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = _FOR;
    node->forIteratorList = list;
    node->cond = range;
    node->cycleSingleStmt = cycleStmt;
    return node;
}

StmtNode *StmtNode::createForNodeFromBlockStmtWithSeveralId(VarDeclarationList *list, ExprNode *range, StmtListNode *cycleStmt) {
    StmtNode* node = new StmtNode();
    node->type = _FOR;
    node->forIteratorList = list;
    node->cond = range;
    node->blockStmts = cycleStmt;
    return node;
}



StmtNode * StmtNode::createReturnNode(ExprNode *expr) {
    StmtNode* node = new StmtNode();
    node->type = _RETURN;
    node->expr = expr;
    return node;
}

string StmtNode::getDotLabel() const {
    return stmtTypeToString(type);
}

StmtNode * StmtNode::createIfNode(ExprNode *ifCond, StmtListNode *trueB, StmtListNode *falseB) {
    StmtNode* node = new StmtNode();
    node->type = _IF_STMT;
    node->cond = ifCond;
    node->trueStmtList = trueB;
    node->falseStmtList = falseB;
    return node;
}


string StmtNode::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, cond, "condition");
    addDotChild(dot, trueStmtList, "true_branch");
    addDotChild(dot, falseStmtList, "false_branch");
    addDotChild(dot, cycleSingleStmt, "cycle");
    addDotChild(dot, blockStmts, "block");
    addDotChild(dot, expr, "expr");
    addDotChild(dot, varDeclaration, "variable declaration");
    addDotChild(dot, forIterator, "iterator");
    addDotChild(dot, forIteratorList, "iteratorList");
    addDotChild(dot, varModifiers, "modifiers");

    return dot;
}



