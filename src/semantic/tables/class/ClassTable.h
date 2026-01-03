#ifndef KOTLIN_COMPILER_CLASSTABLE_H
#define KOTLIN_COMPILER_CLASSTABLE_H
#include <map>
#include <string>

#include "ClassTableElement.h"
#include "topLevel/KotlinFileNode.h"
#include "../../error/SemanticError.h"


class ClassTable {
public:
    std::map<std::string, ClassTableElement*> items;

    SemanticError* buildClassTable(KotlinFileNode* root, const std::string& fileName);

    ClassTable();

    // ------------------- Helpers -----------------

    bool exists(const std::string& name);
    void add(const std::string& name, ClassTableElement* elem);

    // Получить имя класса для top-level (свободных) функций
    static std::string makeTopLevelClassName(const std::string& fileName);
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H