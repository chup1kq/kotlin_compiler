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

    string toDot() const override;
    string getDotLabel() const override;

};


#endif //KOTLIN_COMPILER_KOTLINELEMENTLIST_H