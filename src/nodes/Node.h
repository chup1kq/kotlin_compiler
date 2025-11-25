#ifndef KOTLIN_COMPILER_NODE_H
#define KOTLIN_COMPILER_NODE_H

#include <string>

class ExprNode;
class StmtNode;
using namespace std;

class Node {
protected:
    static unsigned int maxId;

public:
    unsigned int id;
    Node() {id = ++maxId;};
};


#endif //KOTLIN_COMPILER_NODE_H
