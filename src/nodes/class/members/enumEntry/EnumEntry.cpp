#include "EnumEntry.h"

EnumEntry *EnumEntry::createEnumEntry(std::string name, ExprListNode *params) {
    EnumEntry *entry = new EnumEntry();
    entry->name = name;
    entry->params = params;
    return entry;
}
