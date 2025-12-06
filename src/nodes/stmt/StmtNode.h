#ifndef KOTLIN_COMPILER_STMTNODE_H
#define KOTLIN_COMPILER_STMTNODE_H
#include <list>

#include "ModifierMap.h"
#include "../Node.h"
#include "StmtListNode.h"
#include "../types.h"
#include "../varDeclaration/VarDeclaration.h"


class VarDeclarationList;

class StmtNode : public Node {
public:
    StmtType type;
    ExprNode *cond;
    StmtNode *cycleSingleStmt;
    StmtListNode *blockStmts;
    ExprNode *expr;
    VarDeclaration *varDeclaration;
    VarDeclaration *forIterator;
    VarDeclarationList *forIteratorList;
    ModifierMap* varModifiers;

    static StmtNode* createCycleNodeFromSingleStmt(StmtType type, ExprNode* cycleCond, StmtNode* cycleStmt);
    static StmtNode* createCycleNodeFromBlockStmt(StmtType type, ExprNode* cycleCond, StmtListNode* cycleStmt);
    static StmtNode* createForNodeFromSingleStmt(VarDeclaration* iterator, ExprNode* range, StmtNode* cycleStmt);
    static StmtNode* createForNodeFromBlockStmt(VarDeclaration* iterator, ExprNode* range, StmtListNode* cycleStmt);
    static StmtNode* createForNodeFromSingleStmtWithSeveralId(VarDeclarationList* list, ExprNode* range, StmtNode* cycleStmt);
    static StmtNode* createForNodeFromBlockStmtWithSeveralId(VarDeclarationList* list, ExprNode* range, StmtListNode* cycleStmt);
    static StmtNode* createBreakNode();
    static StmtNode* createContinueNode();
    static StmtNode* createEmptyStmt();
    static StmtNode* createStmtFromExpr(ExprNode* expr);
    static StmtNode* createVarOrValStmtNode(StmtType type, VarDeclaration* varDeclaration);
    static StmtNode* createVarOrValStmtNode(ModifierMap* modifiers, StmtType type, VarDeclaration* varDeclaration);
    static StmtNode * createReturnNode(ExprNode* expr);

    string toDot() const override;
    string getDotLabel() const override;
};


#endif //KOTLIN_COMPILER_STMTNODE_H