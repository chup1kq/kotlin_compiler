#ifndef KOTLIN_COMPILER_CLASSNODE_H
#define KOTLIN_COMPILER_CLASSNODE_H
#include <string>

#include "members/ClassBodyNode.h"
#include "members/Constructor.h"
#include "Inheritance.h"
#include "ModifierMap.h"
#include "Node.h"

class ClassNode : public Node {
public:
    std::string name;
    ModifierMap* modifiers;
    Inheritance* inheritance;
    Constructor* primaryConstructor;

    ClassBodyNode* body;

    static ClassNode* createClassNode(ModifierMap* modifiers, std::string name, Constructor* primaryConstructor, Inheritance* inheritance, ClassBodyNode* body);

};


#endif //KOTLIN_COMPILER_CLASSNODE_H