#include "ExprListNode.h"

#include "VarDeclarationList.h"

ExprListNode::ExprListNode(ExprNode *expr) {
    exprs = new std::list<ExprNode*>;
    exprs->push_back(expr);
}

ExprListNode *ExprListNode::addExprToList(ExprListNode *list, ExprNode *expr) {
    if (list == nullptr) {
        return new ExprListNode(expr);
    }

    list->exprs->push_back(expr);
    return list;
}

