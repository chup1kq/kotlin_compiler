#include "FuncParam.h"

FuncParam::FuncParam(std::string name, SemanticType* type)
    : name(std::move(name)),
      type(type)
{
}

/*
 * Равенство параметров:
 * - имя игнорируем
 * - сравниваем ТОЛЬКО тип
 */
bool FuncParam::operator==(const FuncParam& other) const {
    if (type == nullptr || other.type == nullptr)
        return type == other.type;

    return type->isReplaceable(*other.type)
        && other.type->isReplaceable(*type);
}

/*
 * Упорядочивание параметров
 * Нужно для map / set / сортировок
 */
bool FuncParam::operator<(const FuncParam& other) const {
    if (type == nullptr || other.type == nullptr)
        return type < other.type;

    // 1. массивы vs не массивы
    if (type->isArray() != other.type->isArray())
        return type->isArray() < other.type->isArray();

    // 2. размерность массива
    if (type->isArray()) {
        int d1 = type->arrayDimension();
        int d2 = other.type->arrayDimension();
        if (d1 != d2)
            return d1 < d2;
    }

    // 3. имя класса
    if (type->className != other.type->className)
        return type->className < other.type->className;

    // 4. nullable
    if (type->isNullable != other.type->isNullable)
        return type->isNullable < other.type->isNullable;

    return false;
}
