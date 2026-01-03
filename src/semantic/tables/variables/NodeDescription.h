#ifndef KOTLIN_COMPILER_NODEDESCRIPTION_H
#define KOTLIN_COMPILER_NODEDESCRIPTION_H
#include "TypeNodeType.h"

class NodeDescription {
public:
    int id;
    TypeNodeType type;
    char * ident;
    NodeDescription * complexNode;
};

#endif //KOTLIN_COMPILER_NODEDESCRIPTION_H