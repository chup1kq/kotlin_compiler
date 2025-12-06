#include "EnumEntry.h"

EnumEntry *EnumEntry::createEnumEntry(std::string name, ExprListNode *params) {
    EnumEntry *entry = new EnumEntry();
    entry->name = name;
    entry->params = params;
    return entry;
}

string EnumEntry::getDotLabel() const {
    return "Entry: " + name;
}

string EnumEntry::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, params, "params");

    return dot;
}
