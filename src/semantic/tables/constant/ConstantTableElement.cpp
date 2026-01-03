#include "ConstantTableElement.h"

#include "ConstantTableElement.h"

ConstantTableElement::ConstantTableElement(
    int id,
    ConstantType type,
    std::string utf8String,
    int intValue,
    double doubleValue,
    int firstRef,
    int secondRef
) : id(id), type(type), utf8String(utf8String),
    intValue(intValue), doubleValue(doubleValue),
    firstRef(firstRef), secondRef(secondRef) {
}