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
    std::string baseClassName;

    void buildClassTable(KotlinFileNode* root);

    ClassTable(const std::string& fileName);

    // ------------------- Helpers -----------------

    // Получить имя класса для top-level (свободных) функций
    static std::string makeTopLevelClassName(const std::string& fileName);

    void addBaseClass(const std::string& fileName);

    void addClassesToClassTable(ClassTableElement *baseClass, std::list<ClassNode*> classList);

    void attributeAndFillLocalsInClasses();
    void attributeAndFillLocals(MethodTableElement* method);
    void attributeAndFillLocalsInStatement(MethodTableElement* method, StmtNode* stmt);

    void attributeExpression(MethodTableElement* method, ExprNode* expr, bool isStatementContext = false);

    void attributeVarOrValStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeIfStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeCycle(MethodTableElement* method, StmtNode* stmt);
    void attributeFor(MethodTableElement* method, StmtNode* stmt);
    void attributeReturn(MethodTableElement* method, StmtNode* stmt);

    void attributeIdentifierExpr(LocalVariableTable *table, ExprNode* expr);
    void attributeAssignmentExpr(LocalVariableTable *table, ExprNode* expr);
    void attributeArrayCreatingExpr(MethodTableElement *method, ExprNode* expr);
    void attributeFuncOrMethodCall(MethodTableElement* currentMethod, ExprNode* expr);


    // Сгенерировать таблицу методанных
    void initStdClasses();

private:
    bool isNeededType(const std::string& signature, const std::string& type);
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H