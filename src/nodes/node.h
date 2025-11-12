#ifndef KOTLIN_COMPILER_NODE_H
#define KOTLIN_COMPILER_NODE_H

#include <list>
#include <string>

class ExprNode;
class StmtNode;
using namespace std;

#include "types.h"

class Node {
protected:
    static unsigned int maxId;

public:
    unsigned int id;
    Node() {id = ++maxId;};
};

class ExprListNode: public Node {
public:
    list<ExprNode*> *exprs;

    static ExprListNode* addExprToList(ExprListNode *list, ExprNode *expr);
};

class StmtListNode: public Node {
public:
    list<StmtNode*> *stmts;

    static StmtListNode* addStmtToList(StmtListNode *list, StmtNode *stmt);
};

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
    std::string stringValue;

    static ExprNode* createIntNode(int value);
    static ExprNode* createFloatNode(float value);
    static ExprNode* createDoubleNode(double value);
    static ExprNode* createCharNode(char value);
    static ExprNode* createStringNode(std::string value);
    static ExprNode* createBoolNode(bool value);
    static ExprNode* createExprNode(ExprType type, ExprNode* left, ExprNode* right);
};

class StmtNode : public Node {
public:
    StmtType type;
    ExprNode *cond;
    StmtNode *trueStmt, *falseStmt;
    StmtNode *cycleStmt;
    std::list<StmtNode *> blockStmts;

    static StmtNode* createIfNode(ExprNode* ifCond, ExprNode* trueB, ExprNode* falseB);
    static StmtNode* createCycleNode(StmtType  type, ExprNode* cycleCond, StmtNode* cycleStmt);
    static StmtNode* createBreakNode();
    static StmtNode* createContinueNode();
};


#endif //KOTLIN_COMPILER_NODE_H
