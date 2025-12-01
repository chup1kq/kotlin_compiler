#ifndef KOTLIN_COMPILER_INHERITANCE_H
#define KOTLIN_COMPILER_INHERITANCE_H
#include <string>

#include "ExprListNode.h"
#include "Node.h"


class Inheritance : public Node {
public:
    std::string name;
    ExprListNode* constructorParams;

    static Inheritance* createInheritance(string name, ExprListNode* constructorParams);
};


#endif //KOTLIN_COMPILER_INHERITANCE_H