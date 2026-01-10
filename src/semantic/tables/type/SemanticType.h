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

    // Фабричные методы
    static SemanticType* classType(const std::string& className, bool nullable = false);
    static SemanticType* arrayType(SemanticType* elementType, bool nullable = false);

    bool isArray() const;
    int arrayDimension() const;
    bool isReplaceable(const SemanticType& other) const;

    SemanticType();
    explicit SemanticType(TypeNode* typeNode);

    std::string toString() const;
};


#endif //KOTLIN_COMPILER_TYPE_H