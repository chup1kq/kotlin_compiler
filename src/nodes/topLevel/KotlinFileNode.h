#ifndef KOTLIN_COMPILER_KOTLINFILENODE_H
#define KOTLIN_COMPILER_KOTLINFILENODE_H
#include "KotlinElementList.h"


class KotlinFileNode : public Node {
public:
    KotlinElementList* topLevelList;

    KotlinFileNode(KotlinElementList* list);
};


#endif //KOTLIN_COMPILER_KOTLINFILENODE_H