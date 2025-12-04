#ifndef KOTLIN_COMPILER_KOTLINFILENODE_H
#define KOTLIN_COMPILER_KOTLINFILENODE_H
#include "KotlinElementList.h"


class KotlinFileNode : public Node {
public:
    KotlinElementList* topLevelList;

    KotlinFileNode(KotlinElementList* list);

    string toDot() const override;
    string getDotLabel() const override;
};


#endif //KOTLIN_COMPILER_KOTLINFILENODE_H