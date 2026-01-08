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

    void attributeAndFillLocalsInBaseClass(const std::string& topLevelClassName);
    void attributeAndFillLocals(MethodTableElement* method);
    void attributeAndFillLocalsInStatement(MethodTableElement* method, StmtNode* stmt);

    void attributeExpression(MethodTableElement* method, ExprNode* expr);
    void attributeVarOrValStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeIfStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeCycle(MethodTableElement* method, StmtNode* stmt);
    void attributeFor(MethodTableElement* method, StmtNode* stmt);
    void attributeReturn(MethodTableElement* method, StmtNode* stmt);

    // Сгенерировать таблицу методанных
    void initStdClasses();
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H