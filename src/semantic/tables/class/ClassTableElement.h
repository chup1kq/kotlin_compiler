#ifndef KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#define KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#include <string>

#include "ClassNode.h"
#include "../method/MethodTable.h"
#include "../field/FieldTable.h"
#include "../constant/ConstantTable.h"


class ClassParamTable;

class ClassTableElement {
public:
    int name; // ссылка на константу с именем класса - номер константы
    int isOpen; // разрешено ли наследовать данный класс
    int superName; // ссылка на константу с именем родительского класса - номер константы
    int thisClass; // ссылка на константу текущего класса.
    int superClass; // ссылка на константу родительского класса.

    MethodTable* methods;
    FieldTable* fields;

    ConstantTable* constants;

    std::string clsName;

    ClassTableElement();
};

#endif //KOTLIN_COMPILER_CLASSTABLEELEMENT_H
