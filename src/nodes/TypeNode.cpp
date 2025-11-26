#include "TypeNode.h"

TypeNode * TypeNode::createType(Type type, bool isNull) {
    TypeNode* typeNode = new TypeNode();
    typeNode->type = type;
    typeNode->isNull = isNull;
    return typeNode;
}

TypeNode * TypeNode::createArrayType(bool isNull, TypeNode* arrayType) {
    TypeNode* typeNode = new TypeNode();
    typeNode->type = _ARRAY;
    typeNode->isNull = isNull;
    typeNode->arrayType = arrayType;
    return typeNode;
}

TypeNode * TypeNode::makeNullableType(TypeNode* typeNode) {
    typeNode->isNull = true;
    return typeNode;
}