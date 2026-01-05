#ifndef KOTLIN_COMPILER_TYPE_H
#define KOTLIN_COMPILER_TYPE_H
#include "TypeNodeType.h"
#include "NodeDescription.h"
#include "types.h"

class ClassTableElement;

class TypeTable {
public:
    TypeNodeType type;
    ClassTableElement * classTableElement;
    std::string className;
    int arraySize;

    bool isReplaceable(Type & other) const;

    NodeDescription * toDescriptionNode;

    TypeTable();
    TypeTable(NodeDescription * description);
};

#endif //KOTLIN_COMPILER_TYPE_H