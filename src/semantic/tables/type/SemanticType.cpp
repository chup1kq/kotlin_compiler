#include "SemanticType.h"

#include "../class/ClassTable.h"

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
            : SemanticType::classType(typeNode->customName)->className;
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
    if (!isTypeExist(className)) {
        throw SemanticError::unknownType(className);
    }
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

// Массивы не проверяет
bool SemanticType::isTypeExist(std::string type) {
    for (auto& clsType: ClassTable::allClassesTypes) {
        if (clsType == type) {
            return true;
        }
    }

    if (type == "JavaRTL/Int" ||
        type == "JavaRTL/String" ||
        type == "JavaRTL/Char" ||
        type == "JavaRTL/Boolean" ||
        type == "JavaRTL/Float" ||
        type == "JavaRTL/Double" ||
        type == "JavaRTL/Unit" ||
        type == "Unit"
    ) {
        return true;
    }

    return false;
}

std::string SemanticType::isDeclaredClass() const {
    for (auto& clsType: ClassTable::allClassesTypes) {
        if (clsType == this->className) {
            return this->className;
        }
    }

    return "";
}