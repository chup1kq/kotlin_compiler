#include "FieldTable.h"

#include "../../error/SemanticError.h"

FieldTable::FieldTable() {
    this->fields = std::map<std::string, FieldTableElement*>();
}

void FieldTable::addField(const std::string &fieldName, const std::string &descriptor, FieldTableElement *element) {
    if (contains(fieldName)) {
        throw SemanticError::fieldAlreadyExists(fieldName, "");
    }

    fields[fieldName] = element;
}

bool FieldTable::contains(const std::string &filedName) {
    return fields.contains(filedName);
}
