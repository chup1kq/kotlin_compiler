#ifndef KOTLIN_COMPILER_ENUMENTRYLIST_H
#define KOTLIN_COMPILER_ENUMENTRYLIST_H
#include <list>

#include "EnumEntry.h"


class EnumEntryList {
public:
    std::list<EnumEntry*> *enumEntries;

    EnumEntryList(EnumEntry* first);

    static EnumEntryList* addEnumEntry(EnumEntryList* list, EnumEntry* entry);

};


#endif //KOTLIN_COMPILER_ENUMENTRYLIST_H