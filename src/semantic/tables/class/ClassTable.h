#ifndef KOTLIN_COMPILER_CLASSTABLE_H
#define KOTLIN_COMPILER_CLASSTABLE_H
#include <map>
#include <string>

#include "ClassTableElement.h"
#include "topLevel/KotlinFileNode.h"
#include "../../error/SemanticError.h"


class ClassTable {
public:
    static std::map<std::string, ClassTableElement*> items;

    static SemanticError* buildClassTable(KotlinFileNode* root);

    // ------------------- Helpers -----------------

    static bool exists(const std::string& name);
    static void add(const std::string& name, ClassTableElement* elem);

};

#endif //KOTLIN_COMPILER_CLASSTABLE_H