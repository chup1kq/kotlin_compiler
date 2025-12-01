#ifndef KOTLIN_COMPILER_TYPENODE_H
#define KOTLIN_COMPILER_TYPENODE_H
#include "../Node.h"
#include "../types.h"

class TypeNode : public Node {
    public:
        Type type;
        bool isNull;
        TypeNode* arrayType;

    static TypeNode* createType(Type type, bool isNull);
    static TypeNode* createArrayType(bool isNull, TypeNode* arrayType);
    static TypeNode* makeNullableType(TypeNode* typeNode);
};

#endif //KOTLIN_COMPILER_TYPENODE_H
