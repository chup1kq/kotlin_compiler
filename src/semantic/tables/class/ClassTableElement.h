#ifndef KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#define KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#include <string>

#include "ClassNode.h"

class FieldTable;
class MethodTable;
class ConstantTable;
class ClassParamTable;

class ClassTableElement {
public:
    int name; // ссылка на константу с именем класса - номер константы
    int isOpen; // разрешено ли наследовать данный класс
    int superName = NULL; // ссылка на константу с именем родительского класса - номер константы
    int thisClass; // ссылка на константу текущего класса.
    int superClass; // ссылка на константу родительского класса.

    MethodTable* methods;
    FieldTable* fields;

    std::string clsName;

    ClassTableElement();
};

#endif //KOTLIN_COMPILER_CLASSTABLEELEMENT_H
