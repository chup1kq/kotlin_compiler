#include "ExprNode.h"

ExprNode* ExprNode::createIntNode(int value) {
    ExprNode* node = new ExprNode();
    node->type = _INTEGER_LITERAL;
    node->intValue = value;
    return node;
}

ExprNode* ExprNode::createFloatNode(float value) {
    ExprNode* node = new ExprNode();
    node->type = _FLOAT_LITERAL;
    node->floatValue = value;
    return node;
}

ExprNode* ExprNode::createDoubleNode(double value) {
    ExprNode* node = new ExprNode();
    node->type = _DOUBLE_LITERAL;
    node->doubleValue = value;
    return node;
}

ExprNode* ExprNode::createBoolNode(bool value) {
    ExprNode* node = new ExprNode();
    node->type = _BOOL_LITERAL;
    node->boolValue = value;
    return node;
}

ExprNode * ExprNode::createCharNode(char value) {
    ExprNode* node = new ExprNode();
    node->type = _CHAR_LITERAL;
    node->charValue = value;
    return node;
}

ExprNode * ExprNode::createStringNode(std::string value) {
    ExprNode* node = new ExprNode();
    node->type = _STRING_LITERAL;
    node->stringValue = value;
    return node;
}

ExprNode *ExprNode::createNullNode() {
    ExprNode* node = new ExprNode();
    node->type = _NULL_LITERAL;
    return node;
}

ExprNode * ExprNode::createExprNode(ExprType type, ExprNode *left, ExprNode *right) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

ExprNode * ExprNode::createIfNode(ExprNode *ifCond, StmtListNode *trueB, StmtListNode *falseB) {
    ExprNode* node = new ExprNode();
    node->type = _IF_STMT;
    node->cond = ifCond;
    node->trueStmtList = trueB;
    node->falseStmtList = falseB;
    return node;
}

// ExprNode * ExprNode::createIfNode(ExprNode *ifCond, StmtListNode *trueB, StmtNode *falseB) {
// }
//
// ExprNode * ExprNode::createIfNode(ExprNode *ifCond, ExprNode *trueB, StmtListNode *falseB) {
// }
//
// ExprNode * ExprNode::createIfNode(ExprNode *ifCond, StmtNode *trueB, StmtListNode *falseB) {
// }

// ExprNode * ExprNode::createIfNode(ExprNode *ifCond, ExprNode *trueB, StmtNode *falseB) {
//     ExprNode* node = new ExprNode();
//     node->type = _IF_STMT;
//     node->cond = ifCond;
//     node->trueExpr = trueB;
//     node->falseStmt = falseB;
//     return node;
// }
//
// ExprNode * ExprNode::createIfNode(ExprNode *ifCond, StmtNode *trueB, StmtNode *falseB) {
//     ExprNode* node = new ExprNode();
//     node->type = _IF_STMT;
//     node->cond = ifCond;
//     node->trueStmt = trueB;
//     node->falseStmt = falseB;
//     return node;
// }

ExprNode * ExprNode::createMethodAccessExprNode(string name, ExprListNode *params, ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = _FUNC_ACCESS;
    node->identifierName = name;
    node->params = params;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createFieldAccessExprNode(string name, ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = _FIELD_ACCESS;
    node->identifierName = name;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createFunctionCallExprNode(string name, ExprListNode *params) {
    ExprNode* node = new ExprNode();
    node->type = _FUNC_CALL;
    node->identifierName = name;
    node->params = params;
    return node;
}

ExprNode * ExprNode::createAssignmentExprNode(ExprType type, ExprNode *left, ExprNode *right) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

ExprNode * ExprNode::createBracketExprNode(ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = _BRACKETS;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createIDNode(string name) {
    ExprNode* node = new ExprNode();
    node->type = _IDENTIFIER;
    node->identifierName = name;
    return node;
}

ExprNode * ExprNode::createThisExprNode() {
    ExprNode* node = new ExprNode();
    node->type = _THIS;
    return node;
}

ExprNode * ExprNode::createSuperExprNode() {
    ExprNode* node = new ExprNode();
    node->type = _SUPER;
    return node;
}

ExprNode * ExprNode::createRangeExprNode(ExprNode *left, ExprNode *right) {
    ExprNode* node = new ExprNode();
    node->type = _RANGE;
    node->left = left;
    node->right = right;
    return node;
}

ExprNode * ExprNode::createPrefExprNode(ExprType type, ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->right = expr;
    return node;
}

ExprNode * ExprNode::createPostExprNode(ExprType type, ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createUnaryExprNode(ExprType type, ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createNotExprNode(ExprNode *expr) {
    ExprNode* node = new ExprNode();
    node->type = _NOT;
    node->left = expr;
    return node;
}

ExprNode * ExprNode::createArrayExprNode(TypeNode* type, ExprListNode *exprs) {
    ExprNode* node = new ExprNode();
    node->type = _ARRAY_EXPR;
    node->typeElements = type;
    node->elements = exprs;
    return node;
}
