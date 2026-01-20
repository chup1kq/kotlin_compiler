#ifndef KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#define KOTLIN_COMPILER_CLASSTABLEELEMENT_H
#include <string>

#include "ClassNode.h"
#include "../method/MethodTable.h"
#include "../field/FieldTable.h"
#include "../constant/ConstantTable.h"


class ClassParamTable;

class ClassTableElement {
public:
    int name; // ссылка на константу с именем класса - номер константы
    int isOpen; // разрешено ли наследовать данный класс
    int superName; // ссылка на константу с именем родительского класса - номер константы
    std::string superClsName;
    int thisClass; // ссылка на константу текущего класса.
    int superClass; // ссылка на константу родительского класса.

    MethodTable* methods;
    FieldTable* fields;

    ConstantTable* constants;

    ModifierMap* modifiers;

    std::string clsName;

    ClassTableElement();

    void addPrimaryConstructor(Constructor* primaryConstructor);
    void addDefaultConstructorIfNeeded() const;

    void addMethodsToTable(std::list<FunNode *> funcList);
    void addMethodToTable(FunNode* method);
    void addStandardMethodToTable(const std::string& methodName, SemanticType* returnType, const std::string& fullDesc);

    void addFieldsToTable(StmtListNode* fields);
    void addFieldToTable(StmtNode* stmt);

    void checkFiledsModifiers();
    void checkFiledModifier();

    static std::string createMethodDescriptor(vector<FuncParam*> params, SemanticType* returnType);
    static std::string createMethodDescriptor(vector<SemanticType*> params, SemanticType* returnType);
    static std::string createVoidMethodDescriptor(vector<FuncParam*> params);
    static std::string addParamsToMethodDescriptor(vector<FuncParam*> params);
    static std::string addParamsToMethodDescriptor(vector<SemanticType*> params);
    static std::string createTypeDescriptor(SemanticType* type);
};

#endif //KOTLIN_COMPILER_CLASSTABLEELEMENT_H
