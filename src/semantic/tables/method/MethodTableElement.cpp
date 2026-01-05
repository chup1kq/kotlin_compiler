#include "MethodTableElement.h"

MethodTableElement::MethodTableElement(
    int nameConst,
    int descConst,
    std::string name,
    std::string desc
)
    : methodName(nameConst),
      descriptor(descConst),
      strName(std::move(name)),
      strDesc(std::move(desc)),
      start(nullptr),
      retType(nullptr),
      localVarTable(nullptr),
      isFirst(0)
{
}
