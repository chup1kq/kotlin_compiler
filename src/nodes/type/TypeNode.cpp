#include "TypeNode.h"

TypeNode * TypeNode::createType(Type type, bool isNull, std::string customName) {
    TypeNode* typeNode = new TypeNode();
    typeNode->type = type;
    typeNode->customName = customName;
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

string TypeNode::getDotLabel() const {
    if (type == _ARRAY) {
        return typeToString(type) + "<" + this->arrayType->getDotLabel() + ">";
    }

    const std::string returnString = customName.empty() ? typeToString(type) : customName;

    return returnString + (isNull ? " nullable" : " not-nullable");
}

string TypeNode::toDot() const {
    string dot;

    addDotNode(dot);

    return dot;
}
