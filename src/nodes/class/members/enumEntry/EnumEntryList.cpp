#include "EnumEntryList.h"

EnumEntryList::EnumEntryList(EnumEntry *first) {
    enumEntries = new std::list<EnumEntry*>();
    enumEntries->push_back(first);
}

EnumEntryList *EnumEntryList::addEnumEntry(EnumEntryList *list, EnumEntry *entry) {
    if (list == nullptr) {
        return new EnumEntryList(entry);
    }

    list->enumEntries->push_back(entry);
    return list;
}

