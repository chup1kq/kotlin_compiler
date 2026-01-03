#ifndef KOTLIN_COMPILER_TYPE_H
#define KOTLIN_COMPILER_TYPE_H
#include "TypeNodeType.h"
#include "../class/ClassTableElement.h"
#include "NodeDescription.h"

class TypeTable {
public:
    TypeNodeType type;
    ClassTableElement * classTableElement;
    std::string className;
    int arraySize;

    bool isReplaceable(Type & other) const;
    NodeDescription * toDescriptionNode;

    Type();
    Type(NodeDescription * description);
};

#endif //KOTLIN_COMPILER_TYPE_H