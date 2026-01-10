#include "SemanticType.h"

SemanticType::SemanticType()
    : isNullable(false),
      classTableElement(nullptr),
      className(""),
      elementType(nullptr)
{
}


SemanticType::SemanticType(TypeNode* typeNode)
    : isNullable(false),
      classTableElement(nullptr),
      className(""),
      elementType(nullptr)
{
    isNullable = typeNode->isNullable;

    if (typeNode->isArray) {
        elementType = new SemanticType(typeNode->arrayType);
    } else {
        className = typeNode->customName.empty()
            ? typeToString(typeNode->type)
            : typeNode->customName;
    }
}


bool SemanticType::isArray() const {
    return elementType != nullptr;
}

int SemanticType::arrayDimension() const {
    if (!isArray()) return 0;
    return 1 + elementType->arrayDimension();
}

bool SemanticType::isReplaceable(const SemanticType& other) const {
    if (isArray() != other.isArray())
        return false;

    if (isArray()) {
        return elementType->isReplaceable(*other.elementType);
    }

    // TODO учитывать, что null нельзя присвоить
    // TODO учитывать наследование через classTableElement
    return className == other.className;
}

SemanticType* SemanticType::classType(const std::string& className, bool nullable) {
    SemanticType* t = new SemanticType();
    t->className = className;
    t->isNullable = nullable;
    return t;
}

SemanticType* SemanticType::arrayType(SemanticType* elementType, bool nullable) {
    SemanticType* t = new SemanticType();
    t->elementType = elementType;
    t->isNullable = nullable;
    return t;
}

std::string SemanticType::toString() const {
    if (!isArray()) {
        return className.empty() ? "Undefined" : className;
    }

    return "Array<" + elementType->toString() + ">";
}

