#ifndef KOTLIN_COMPILER_ENUMENTRY_H
#define KOTLIN_COMPILER_ENUMENTRY_H
#include <string>

#include "ExprListNode.h"


class EnumEntry {
public:
    std::string name;
    ExprListNode* params;

    static EnumEntry* createEnumEntry(std::string name, ExprListNode* params);

};


#endif //KOTLIN_COMPILER_ENUMENTRY_H