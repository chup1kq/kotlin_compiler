#ifndef KOTLIN_COMPILER_KOTLINELEMENTLIST_H
#define KOTLIN_COMPILER_KOTLINELEMENTLIST_H
#include <list>

#include "ClassNode.h"


class KotlinElementList : public Node  {
public:
    std::list<ClassNode*> classList;
    std::list<FunNode*> functionList;

    static KotlinElementList* addElement(KotlinElementList* list, ClassNode* classNode);
    static KotlinElementList* addElement(KotlinElementList* list, FunNode* functionNode);

};


#endif //KOTLIN_COMPILER_KOTLINELEMENTLIST_H