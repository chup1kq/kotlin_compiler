#ifndef KOTLIN_COMPILER_STMTLISTNODE_H
#define KOTLIN_COMPILER_STMTLISTNODE_H
#include <list>

#include "../Node.h"


class StmtListNode: public Node {
public:
    std::list<StmtNode*> *stmts;

    static StmtListNode* addStmtToList(StmtListNode *list, StmtNode *stmt);
};


#endif //KOTLIN_COMPILER_STMTLISTNODE_H