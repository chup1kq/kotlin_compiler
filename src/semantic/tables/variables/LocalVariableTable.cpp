#include "LocalVariableTable.h"

int LocalVariableTable::findOrAddLocalVar(std::string name, SemanticType *type, int isConst, int isInitialized) {

    if (items.find(name) == items.end()) {
        items[name] = new LocalVariableTableElement(name, maxId++, type, isConst, isInitialized);
        return items[name]->id;
    }

    return items[name]->id;
}

bool LocalVariableTable::contains(std::string name) {
    return items.find(name) != items.end();
}
