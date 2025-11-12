#ifndef KOTLIN_COMPILER_NODE_H
#define KOTLIN_COMPILER_NODE_H

#include <list>
#include <string>

#include "types.h"

class ExprNode {
public:
    ExprType type;
    ExprNode* left;
    ExprNode* right;
    int intValue;
    float floatValue;
    double doubleValue;
    bool boolValue;
    char charValue;
    std::string stringValue;

    static ExprNode* createIntNode(int value);
    static ExprNode* createFloatNode(float value);
    static ExprNode* createDoubleNode(double value);
    static ExprNode* createCharNode(char value);
    static ExprNode* createStringNode(std::string value);
    static ExprNode* createBoolNode(bool value);
    static ExprNode* createExprNode(ExprType type, ExprNode* left, ExprNode* right);
    static ExprNode* createAssignmentNode(ExprType type, ExprNode* left, ExprNode* right);
};

class StmtNode {
public:
    StmtType type;
    ExprNode *cond;
    StmtNode *trueStmt, *falseStmt;
    StmtNode *cycleStmt;
    std::list<StmtNode *> blockStmts;

    static StmtNode* createIfNode(ExprNode* ifCond, ExprNode* trueB, ExprNode* falseB);
    static StmtNode* createCycleNode(StmtType  type, ExprNode* cycleCond, StmtNode* cycleStmt);
};


#endif //KOTLIN_COMPILER_NODE_H
