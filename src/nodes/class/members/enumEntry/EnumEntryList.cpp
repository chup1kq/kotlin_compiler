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

string EnumEntryList::getDotLabel() const {
    return "EnumEntryList";
}

string EnumEntryList::toDot() const {
    string dot;

    addDotNode(dot);
    if (!enumEntries->empty()) {
        int i = 0;
        for (const auto *it : *enumEntries) {
            addDotChild(dot, it, "entry_" + to_string(i));
            i++;
        }
    }

    return dot;
}
