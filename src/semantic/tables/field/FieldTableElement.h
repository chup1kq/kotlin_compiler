#ifndef KOTLIN_COMPILER_FIELDTABLEELEMENT_H
#define KOTLIN_COMPILER_FIELDTABLEELEMENT_H
#include <string>

#include "ModifierMap.h"

class FieldTableElement {
public:
    int name; // ссылка на константу с именем поля
    int descriptor; // ссылка на константу с дескриптором поля
    std::string strName;
    std::string strDesc;
    ModifierMap* modifierMap; // список модификаторов поля
    int isConst; // является ли указанное поле изменяемым
    ExprNode* fieldValue;

    FieldTableElement(int name,
                      int descriptor,
                      std::string strName,
                      std::string strDesc,
                      ModifierMap* modifierMap,
                      bool isConst,
                      ExprNode* fieldValue);
};

#endif //KOTLIN_COMPILER_FIELDTABLEELEMENT_H