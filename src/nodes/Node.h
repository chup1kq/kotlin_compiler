#ifndef KOTLIN_COMPILER_NODE_H
#define KOTLIN_COMPILER_NODE_H

#include <string>

class ExprNode;
class StmtNode;
using namespace std;

class Node {
protected:
    static unsigned int maxId;

    void addDotNode(string &dot) const;
    void addDotChild(string &dot, const Node* child, const string &arrowLabel) const;

public:
    unsigned int id;
    Node() {id = ++maxId;};

    virtual string toDot() const;
    virtual string getDotLabel() const;
};


#endif //KOTLIN_COMPILER_NODE_H
