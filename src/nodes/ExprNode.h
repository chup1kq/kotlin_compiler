#ifndef KOTLIN_COMPILER_EXPRNODE_H
#define KOTLIN_COMPILER_EXPRNODE_H
#include "Node.h"
#include "types.h"
#include "ExprListNode.h"


class ExprNode : public Node {
public:
    ExprType type;
    ExprNode* left;
    ExprNode* right;
    int intValue;
    float floatValue;
    double doubleValue;
    bool boolValue;
    char charValue;
    string stringValue;
    ExprNode *cond;
    ExprNode *trueExpr, *falseExpr;
    ExprListNode* params;
    string functionName;

    static ExprNode* createIntNode(int value);
    static ExprNode* createFloatNode(float value);
    static ExprNode* createDoubleNode(double value);
    static ExprNode* createCharNode(char value);
    static ExprNode* createStringNode(string value);
    static ExprNode* createBoolNode(bool value);
    static ExprNode* createExprNode(ExprType type, ExprNode* left, ExprNode* right);
    static ExprNode* createIfNode(ExprNode* ifCond, ExprNode* trueB, ExprNode* falseB);
    static ExprNode* createFuncCallExprNode(string name, ExprListNode* params);
};



#endif //KOTLIN_COMPILER_EXPRNODE_H