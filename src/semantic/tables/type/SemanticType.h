#ifndef KOTLIN_COMPILER_TYPE_H
#define KOTLIN_COMPILER_TYPE_H
#include "TypeNode.h"
#include "types.h"

class ClassTableElement;

class SemanticType {
public:
    bool isNullable = false;

    // Если НЕ массив
    ClassTableElement* classTableElement = nullptr;
    std::string className;

    // Если массив
    SemanticType* elementType = nullptr; // тип элемента массива

    bool isArray() const;
    int arrayDimension() const;
    bool isReplaceable(const SemanticType& other) const;

    SemanticType();
    explicit SemanticType(TypeNode* typeNode);
};


#endif //KOTLIN_COMPILER_TYPE_H