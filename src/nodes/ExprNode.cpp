#include "ExprNode.h"

ExprNode* ExprNode::createIntNode(int value) {
    ExprNode* node = new ExprNode();
    node->type = INTEGER_LITERAL;
    node->intValue = value;
    return node;
}

ExprNode* ExprNode::createFloatNode(float value) {
    ExprNode* node = new ExprNode();
    node->type = FLOAT_LITERAL;
    node->floatValue = value;
    return node;
}

ExprNode* ExprNode::createDoubleNode(double value) {
    ExprNode* node = new ExprNode();
    node->type = DOUBLE_LITERAL;
    node->doubleValue = value;
    return node;
}

ExprNode* ExprNode::createBoolNode(bool value) {
    ExprNode* node = new ExprNode();
    node->type = BOOL_LITERAL;
    node->boolValue = value;
    return node;
}

ExprNode * ExprNode::createCharNode(char value) {
    ExprNode* node = new ExprNode();
    node->type = CHAR_LITERAL;
    node->charValue = value;
    return node;
}

ExprNode * ExprNode::createStringNode(std::string value) {
    ExprNode* node = new ExprNode();
    node->type = STRING_LITERAL;
    node->stringValue = value;
    return node;
}

ExprNode * ExprNode::createExprNode(ExprType type, ExprNode *left, ExprNode *right) {
    ExprNode* node = new ExprNode();
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

ExprNode * ExprNode::createIfNode(ExprNode *ifCond, ExprNode *trueB, ExprNode *falseB) {
    ExprNode* node = new ExprNode();
    node->type = IF_STMT;
    node->cond = ifCond;
    node->trueExpr = trueB;
    node->falseExpr = falseB;
    return node;
}

ExprNode * ExprNode::createFuncCallExprNode(string name, ExprListNode *params) {
    ExprNode* node = new ExprNode();
    node->type = FUNC_CALL;
    node->functionName = name;
    node->params = params;
    return node;
}
