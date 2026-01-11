#include "TypeNode.h"

TypeNode* TypeNode::clone() const {
    TypeNode* cloned = new TypeNode();

    cloned->type = this->type;
    cloned->isCustomType = this->isCustomType;
    cloned->customName = this->customName;
    cloned->isNullable = this->isNullable;
    cloned->isArray = this->isArray;

    if (this->arrayType) {
        cloned->arrayType = this->arrayType->clone();
    } else {
        cloned->arrayType = nullptr;
    }

    return cloned;
}


TypeNode * TypeNode::createType(Type type, bool isNullable, std::string customName) {
    TypeNode* typeNode = new TypeNode();
    typeNode->type = type;
    typeNode->customName = customName;
    typeNode->isNullable = isNullable;
    typeNode->isArray = false;
    typeNode->isCustomType = !customName.empty();
    return typeNode;
}

TypeNode* TypeNode::createUndefinedType() {
    return createType(_UNDEFINED, false, "");
}

TypeNode * TypeNode::createArrayType(bool isNullable, TypeNode* arrayType) {
    TypeNode* typeNode = new TypeNode();
    typeNode->type = _ARRAY;
    typeNode->isNullable = isNullable;
    typeNode->arrayType = arrayType;
    typeNode->isArray = true;
    return typeNode;
}

TypeNode * TypeNode::makeNullableType(TypeNode* typeNode) {
    typeNode->isNullable = true;
    return typeNode;
}

string TypeNode::getDotLabel() const {
    if (type == _ARRAY) {
        return typeToString(type) + "<" + this->arrayType->getDotLabel() + ">";
    }

    const std::string returnString = customName.empty() ? typeToString(type) : customName;

    return returnString + (isNullable ? " nullable" : " not-nullable");
}

string TypeNode::toDot() const {
    string dot;

    addDotNode(dot);

    return dot;
}
