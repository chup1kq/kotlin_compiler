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
    static std::map<std::string, ClassTableElement*> items;
    std::string topLevelClassName;
    std::vector<std::string> builtinFunctionClasses;

    static std::vector<std::string> allClassesTypes;

    void buildClassTable(KotlinFileNode* root);

    ClassTable(const std::string& fileName);

    // ------------------- Helpers -----------------

    // Получить имя класса для top-level (свободных) функций
    static std::string makeTopLevelClassName(const std::string& fileName);

    void addBaseClass();

    void addClassesToClassTable(ClassTableElement *baseClass, std::list<ClassNode*> classList);

    void setInheritanceToClasses(std::list<ClassNode*> classList);

    void checkFieldsModifiers();
    void checkMethodsModifiers();

    void attributeAndFillLocalsInClasses();
    void attributeAndFillLocals(MethodTableElement* method);
    void attributeAndFillLocalsForMethodParams(MethodTableElement* method, std::vector<FuncParam *> params);
    void attributeAndFillLocalsInStatement(MethodTableElement* method, StmtNode* stmt);

    void attributeExpression(MethodTableElement* method, ExprNode* expr, bool isStatementContext = false);

    void attributeVarOrValStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeIfStmt(MethodTableElement* method, StmtNode* stmt);
    void attributeCycle(MethodTableElement* method, StmtNode* stmt);
    void attributeCondition(MethodTableElement *method, ExprNode* cond);
    void attributeFor(MethodTableElement* method, StmtNode* stmt);
    void attributeReturn(MethodTableElement* method, StmtNode* stmt);

    void attributeIdentifierExpr(MethodTableElement* method, ExprNode* expr);
    void attributeAssignmentExpr(LocalVariableTable *table, ExprNode* expr);
    void attributeArrayCreatingExpr(MethodTableElement *method, ExprNode* expr);
    void attributeFuncOrMethodCall(MethodTableElement* currentMethod, ExprNode* expr);
    void attributeArrayAccess(MethodTableElement* currentMethod, ExprNode* expr);
    void attributeFieldAccess(MethodTableElement* currentMethod, ExprNode* expr);
    bool isMethodBaseClassConstructorOrInputOutput(ExprNode* expr);

    FieldTableElement* hasSuperClassesField(MethodTableElement* method, std::string fieldName);

    void fillConstructorMethodRefs(ClassTableElement* cls);

    void fillLiterals(ClassTableElement* elem);
    void fillFieldConstants(ClassTableElement* elem);
    void fillLiteralsInStatement(StmtNode* stmt, ClassTableElement* elem);
    void fillLiteralsInExpression(ExprNode* expr, ClassTableElement* elem);

    void fillMethodRefs(ClassTableElement* elem);
    void fillMethodRefsInStatement(StmtNode* stmt, ClassTableElement* elem);
    void fillMethodRefsInExpression(ExprNode* expr, ClassTableElement* elem);

    // Сгенерировать таблицу методанных
    void initStdClasses();

    // Проверка на стандартный класс
    bool isBuiltinClass(const std::string& className);

private:
    bool isNeededType(const std::string& signature, const std::string& type);
    SemanticType checkSameElementsType(std::list<ExprNode*> *exprs);
    std::string getTypeDescriptor(SemanticType* type);
};

#endif //KOTLIN_COMPILER_CLASSTABLE_H