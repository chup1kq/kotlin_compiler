#include "SemanticType.h"

SemanticType::SemanticType()
    : isNullable(false),
      classTableElement(nullptr),
      className(""),
      elementType(nullptr)
{
}


SemanticType::SemanticType(TypeNode* typeNode)
    : isNullable(typeNode->isNullable),
      classTableElement(nullptr),
      className(""),
      elementType(nullptr)
{
    if (typeNode->isArray) {
        // рекурсивно создаём тип элемента
        elementType = new SemanticType(typeNode->arrayType);
    } else {
        // обычный (не массив)
        className = typeNode->customName.empty()
            ? typeToString(typeNode->type)
            : typeNode->customName;
    }
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



