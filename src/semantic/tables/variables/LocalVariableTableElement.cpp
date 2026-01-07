#include "LocalVariableTableElement.h"

LocalVariableTableElement::LocalVariableTableElement(
    std::string name, int id, SemanticType *type, int isConst, int isInitialized) {
    this->name = name;
    this->id = id;
    this->type = type;
    this->isConst = isConst;
    this->isInitialized = isInitialized;
}
