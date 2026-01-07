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

    bool exists(const std::string& name);
    void add(const std::string& name, ClassTableElement* elem);

    // Получить имя класса для top-level (свободных) функций
    static std::string makeTopLevelClassName(const std::string& fileName);

    void addBaseClass(const std::string& fileName);

    static void addTopLevelFunctionsToBaseClass(ClassTableElement *baseClass, std::list<FunNode*> funcList);
    static std::string createMethodDescriptor(vector<FuncParam*> params, SemanticType* returnType);

    void addClassesToClassTable(ClassTableElement *baseClass, std::list<ClassNode*> classList);
    void addPrimaryConstructor(ClassTableElement cls, ClassNode* classNode);

    // Сгенерировать таблицу методанных
    void initStdClasses();
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H