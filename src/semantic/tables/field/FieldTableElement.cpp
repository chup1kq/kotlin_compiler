#include "FieldTableElement.h"

FieldTableElement::FieldTableElement(
    int name,
    int descriptor,
    std::string strName,
    std::string strDesc,
    ModifierMap* modifierMap,
    bool isConst
)
    : name(name),
      descriptor(descriptor),
      strName(std::move(strName)),
      strDesc(std::move(strDesc)),
      modifierMap(modifierMap),
      isConst(isConst ? 1 : 0)
{
}