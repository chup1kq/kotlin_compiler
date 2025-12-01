#ifndef KOTLIN_COMPILER_CLASSBODYNODE_H
#define KOTLIN_COMPILER_CLASSBODYNODE_H
#include <list>

#include "Constructor.h"
#include "FunNode.h"

class ClassBodyNode : public Node {
public:
    std::list<Constructor*> constructors;
    StmtListNode* fields;
    std::list<FunNode*> methods;


};


#endif //KOTLIN_COMPILER_CLASSBODYNODE_H