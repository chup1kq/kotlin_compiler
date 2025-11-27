#include "StmtListNode.h"

StmtListNode::StmtListNode(StmtNode *first) {
    stmts = new std::list<StmtNode*>();
    stmts->push_back(first);
}


