#include "FieldTable.h"

FieldTable::FieldTable() {
    this->fields = std::map<std::string, FieldTableElement*>();
}
