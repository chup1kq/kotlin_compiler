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


string ExprListNode::getDotLabel() const {
    return "ExprListNode";
}

string ExprListNode::toDot() const {
    string dot;

    addDotNode(dot);
    if (!exprs->empty()) {
        int i = 0;
        for (const auto *it : *exprs) {
            addDotChild(dot, it, "expr_" + to_string(it->id));
        }
    }

    return dot;
}
