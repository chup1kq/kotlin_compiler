#ifndef KOTLIN_COMPILER_CLASSTABLE_H
#define KOTLIN_COMPILER_CLASSTABLE_H
#include <map>
#include <string>

#include "ClassTableElement.h"
#include "topLevel/KotlinFileNode.h"
#include "../../error/SemanticError.h"
#include "../type/SemanticType.h"


class ClassTable {
public:
    std::map<std::string, ClassTableElement*> items;

    void buildClassTable(KotlinFileNode* root, const std::string& fileName);

    ClassTable();

    // ------------------- Helpers -----------------

    // Получить имя класса для top-level (свободных) функций
    static std::string makeTopLevelClassName(const std::string& fileName);

    void addBaseClass(const std::string& fileName);

    void addClassesToClassTable(ClassTableElement *baseClass, std::list<ClassNode*> classList);

    // Сгенерировать таблицу методанных
    void initStdClasses();
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H