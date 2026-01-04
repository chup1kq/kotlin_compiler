#ifndef KOTLIN_COMPILER_EXPRNODE_H
#define KOTLIN_COMPILER_EXPRNODE_H
#include "../Node.h"
#include "../types.h"
#include "ExprListNode.h"
#include "../type/TypeNode.h"


class StmtListNode;

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
    ExprListNode* params;
    string identifierName;
    ExprListNode* elements;
    TypeNode* typeElements;

    ExprNode();

    static ExprNode* createIntNode(int value);
    static ExprNode* createFloatNode(float value);
    static ExprNode* createDoubleNode(double value);
    static ExprNode* createCharNode(char value);
    static ExprNode* createStringNode(string value);
    static ExprNode* createBoolNode(bool value);
    static ExprNode* createNullNode();
    static ExprNode* createExprNode(ExprType type, ExprNode* left, ExprNode* right);
    static ExprNode* createMethodAccessExprNode(string name, ExprListNode* params, ExprNode* expr);
    static ExprNode* createSafeMethodAccessExprNode(string name, ExprListNode* params, ExprNode* expr);
    static ExprNode* createFieldAccessExprNode(string name, ExprNode* expr);
    static ExprNode* createSafeFieldAccessExprNode(string name, ExprNode* expr);
    static ExprNode* createFunctionCallExprNode(string name, ExprListNode* params);
    static ExprNode* createAssignmentExprNode(ExprType type, ExprNode* left, ExprNode* right);
    static ExprNode* createBracketExprNode(ExprNode* expr);
    static ExprNode* createIDNode(string name);
    static ExprNode* createThisExprNode();
    static ExprNode* createSuperExprNode();
    static ExprNode* createRangeExprNode(ExprNode* left, ExprNode* right);
    static ExprNode* createPrefExprNode(ExprType type, ExprNode* expr);
    static ExprNode* createPostExprNode(ExprType type, ExprNode* expr);
    static ExprNode* createUnaryExprNode(ExprType type, ExprNode* expr);
    static ExprNode* createNotExprNode(ExprNode* expr);
    static ExprNode* createArrayExprNode(TypeNode* type, ExprListNode* exprs);
    static ExprNode* createArrayAccessNode(ExprNode* arrayName, ExprNode* id);

    string toDot() const override;
    string getDotLabel() const override;
};



#endif //KOTLIN_COMPILER_EXPRNODE_H