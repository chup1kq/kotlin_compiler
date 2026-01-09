#include "ClassTable.h"

#include <iostream>

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

class FuncParam;

ClassTable::ClassTable(const std::string& fileName) {
    this->items.clear();
    this->topLevelClassName = makeTopLevelClassName(fileName);
}


void ClassTable::buildClassTable(KotlinFileNode* root) {
    // Описываем все базовые классы
    initStdClasses();

    // Создаем базовый класс
    addBaseClass();

    // Добавляем методы все top level методы в базовый класс файла
    if (root->topLevelList->functionList)
        items[topLevelClassName]->addMethodsToTable(*root->topLevelList->functionList);

    // Добавляем все классы из файла в таблицу классов
    if (root->topLevelList->classList)
        addClassesToClassTable(items[topLevelClassName], *root->topLevelList->classList);

    // Проверяем и заполняем локальные переменные в top level функциях
    attributeAndFillLocalsInClasses();


    // TODO дописать
}

void ClassTable::addBaseClass() {
    // Cоздание класса для свободных функций
    ClassTableElement* topLevelFunctionsClass = new ClassTableElement();
    topLevelFunctionsClass->clsName = topLevelClassName;

    // Добавляем название и связываем его с классом
    // TODO вот это дублируется с методом addClassesToClassTable
    int topLevelUtf8 = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, topLevelClassName);
    int cls = topLevelFunctionsClass->constants->findOrAddConstant(Class, "" ,0, 0, topLevelUtf8);

    topLevelFunctionsClass->name = topLevelUtf8;
    topLevelFunctionsClass->thisClass = cls;

    // Добавляем название родителя и связываем его с классом
    int parent = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, "java/lang/Object");
    int parentClass = topLevelFunctionsClass->constants->findOrAddConstant(Class, "", 0, 0, parent);

    topLevelFunctionsClass->superName = parent;
    topLevelFunctionsClass->superClass = parentClass;

    // Добавляем базовый класс в таблицу
    items[topLevelClassName] = topLevelFunctionsClass;
}

std::string ClassTable::makeTopLevelClassName(const std::string& fileName) {
    if (fileName.empty())
        return "";

    std::string path = fileName;

    // Убираем путь
    size_t sep = path.find_last_of("\\/");
    if (sep != std::string::npos)
        path = path.substr(sep + 1);

    // Находим расширение
    sep = path.find_last_of('.');
    if (sep == std::string::npos || sep + 1 >= path.size())
        return path;

    // Делаем первую букву имени и расширения заглавной
    path[0] = static_cast<char>(std::toupper(path[0]));
    path[sep + 1] = static_cast<char>(std::toupper(path[sep + 1]));

    // Удаляем точку
    path.erase(sep, 1);

    return path;
}

void ClassTable::addClassesToClassTable(ClassTableElement *baseClass, std::list<ClassNode *> classList) {
    for (auto* classNode : classList) {
        std::string className = classNode->name;

        if (this->items.find(className) != this->items.end()) {
            throw SemanticError::classAlreadyExists(className);
        }

        if (baseClass->methods->methods.contains(className)) {
            throw SemanticError::topLevelMethodAlreadyExists(className);
        }

        ClassTableElement* newClass = new ClassTableElement();
        newClass->isOpen = classNode->modifiers->modifiers->at("inheritance") == OPEN; // по умолчанию final
        newClass->clsName = className;

        int utf8 = newClass->constants->findOrAddConstant(UTF8, className);
        int cls = newClass->constants->findOrAddConstant(Class, "", 0, 0, utf8);

        newClass->name = utf8;
        newClass->thisClass = cls;

        this->items.insert(std::pair<std::string, ClassTableElement*>(className, newClass));

        // TODO добавить primary конструктор
        newClass->addPrimaryConstructor(classNode->primaryConstructor);
        // TODO заполнить поля


        // TODO заполнить методы
        newClass->addMethodsToTable(*classNode->body->methods);
    }
}

void ClassTable::attributeAndFillLocalsInClasses() {
    // По всем клаасам
    for (auto& cls : items) {
        // По всем методам
        for (auto& [methodName, overloads] : cls.second->methods->methods) {
            // По всем перегрузкам
            for (auto& [descriptor, method] : overloads) {
                // method имеет тип MethodTableElement*
                attributeAndFillLocals(method);
            }
        }
    }
}

void ClassTable::attributeAndFillLocals(MethodTableElement *method) {
    if (!method || !method->start || !method->start->stmts)
        return;

    for (auto* stmt : *method->start->stmts) {
        attributeAndFillLocalsInStatement(method, stmt);
    }
}

void ClassTable::attributeAndFillLocalsInStatement(MethodTableElement *method, StmtNode *stmt) {
    if (!stmt)
        return;

    switch (stmt->type) {
        case (_EXPRESSION):
            attributeExpression(method, stmt->expr, true);
            break;
        case (_VAL):
        case (_VAR):
            // attributeVarOrValStmt(method, stmt);
            break;
        case (_IF_STMT):
            attributeIfStmt(method, stmt);
            break;
        case (_WHILE):
        case (_DO_WHILE):
            attributeCycle(method, stmt);
            break;
        case (_FOR):
            attributeFor(method, stmt);
            break;
        case (_RETURN):
            attributeReturn(method, stmt);
            break;
    }
}

void ClassTable::attributeVarOrValStmt(MethodTableElement *method, StmtNode *stmt) {
    VarDeclaration* decl = stmt->varDeclaration;


    const std::string& name = stmt->varDeclaration->varId;

    if (method->localVarTable->contains(name)) {
        throw SemanticError::redefinition(name);
    }

    bool isConst = (stmt->type == _VAL);
    bool isInitialized = (stmt->expr != nullptr);

    if (isInitialized) {
        if (stmt->expr->type == _ASSIGNMENT) {
            throw SemanticError::unallowedAssignment();
        }

        attributeExpression(method, stmt->expr);
    }
    /* TODO доделать */
    // SemanticType varType(stmt->varDec);
    //
    // if (isInitialized) {
    //     if (!varType.isReplaceable(*decl->expr->semanticType)) {
    //         throw SemanticError::assignmentTypeMismatch(stmt->);
    //     }
    // }
    //
    // method->localVarTable->findOrAddLocalVar(
    //     name,
    //     varType,
    //     isConst,
    //     isInitialized
    // );
}

void ClassTable::attributeIfStmt(MethodTableElement *method, StmtNode *stmt) {
    if (!stmt->cond) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }

    attributeExpression(method, stmt->cond);

    if (!isNeededType("LJavaRTL/Boolean", stmt->cond->semanticType->className)) {
        throw SemanticError::conditionNotBoolean(method->strName + method->strDesc);
    }

    if (stmt->trueStmtList && stmt->trueStmtList->stmts) {
        for (auto & stmt: *stmt->trueStmtList->stmts) {
            attributeAndFillLocalsInStatement(method, stmt);
        }
    }

    if (stmt->falseStmtList && stmt->falseStmtList->stmts) {
        for (auto & stmt: *stmt->falseStmtList->stmts) {
            attributeAndFillLocalsInStatement(method, stmt);
        }
    }
}

void ClassTable::attributeCycle(MethodTableElement *method, StmtNode *stmt) {
    if (stmt->cond == nullptr) {
        throw SemanticError::returnTypeMismatch(
            method->strName + method->strDesc
        );
    }

    attributeExpression(method, stmt->cond);

    if (stmt->cond->semanticType == nullptr) {
        throw SemanticError::returnTypeMismatch(
            method->strName + method->strDesc
        );
    }

    if (!isNeededType("LJavaRTL/Boolean", stmt->cond->semanticType->className)) {
        throw SemanticError::conditionNotBoolean(method->strName + method->strDesc);
    }

    if (stmt->blockStmts != nullptr) {
        for (auto* stmt : *stmt->blockStmts->stmts) {
            attributeAndFillLocalsInStatement(method, stmt);
        }
    }
}

/* TODO переделать на forIteratorList */
void ClassTable::attributeFor(MethodTableElement *method, StmtNode *stmt) {
    int varDeclCount = stmt->forIteratorList->decls->size();

    if (varDeclCount > 1) {
        throw SemanticError::multivarForLoop(method->strName + method->strDesc);
    }

    attributeExpression(method, stmt->cond);

    if (stmt->cond->semanticType == nullptr || stmt->cond->type != _ARRAY_EXPR) {
        throw SemanticError::forLoopNotArray(method->strName + method->strDesc);
    }

    VarDeclaration* iter = stmt->forIterator;

    if (iter != nullptr) {
        if (iter->varType != nullptr) {
            SemanticType declaredType(iter->varType);
            SemanticType arrayType(*stmt->cond->semanticType);
            SemanticType elementType = *arrayType.elementType;

            if (!declaredType.isReplaceable(elementType)) {
                throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
            }

            method->localVarTable->findOrAddLocalVar(iter->varType->customName, new SemanticType(declaredType), false, true);
        }
        else {
            SemanticType elementType = *stmt->cond->semanticType;
            method->localVarTable->findOrAddLocalVar(iter->varType->customName, new SemanticType(elementType), false, true);
        }

        std::string imageVar = "$" + iter->varType->customName;

        method->localVarTable->findOrAddLocalVar(imageVar, new SemanticType(), false, true);
    }

    if (stmt->blockStmts != nullptr) {
        for (auto* s : *stmt->blockStmts->stmts) {
            attributeAndFillLocalsInStatement(method, s);
        }
    }
}

void ClassTable::attributeReturn(MethodTableElement *method, StmtNode *stmt) {
    SemanticType* retType = method->retType;

    if (retType == nullptr) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }

    // return;
    if (stmt->expr == nullptr) {
        if (!isNeededType("LUnit", retType->className)) {
            throw SemanticError::missingReturnValue(method->strName + method->strDesc);
        }
        return;
    }

    attributeExpression(method, stmt->expr);

    // return expr;
    if (isNeededType("LUnit", retType->className)) {
        throw SemanticError::returnFromVoid(method->strName + method->strDesc);
    }

    if (!retType->isReplaceable(*stmt->expr->semanticType)) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }
}

void ClassTable::attributeExpression(MethodTableElement *method, ExprNode *expr, bool isStatementContext) {
    if (!expr)
        return;

    // Присваивание может быть только в начале expr, дальше нет
    if (expr->type == _ASSIGNMENT && !isStatementContext)
        throw SemanticError::unallowedAssignment();

    // Семантический тип уже был определен
    if (expr->semanticType)
        return;

    SemanticType* leftType = nullptr;
    SemanticType* rightType = nullptr;

    // Рекурсивный обход
    if (expr->left) {
        attributeExpression(method, expr->left, false);
        leftType = expr->left->semanticType;
    }

    if (expr->right) {
        attributeExpression(method, expr->right, false);
        rightType = expr->right->semanticType;
    }

    switch (expr->type) {
        case _BRACKETS:
            expr->semanticType = leftType;
            return;
        case _IDENTIFIER: {
            attributeIdentifierExpr(method->localVarTable, expr);
            return;
        }
        case _ASSIGNMENT: {
            attributeAssignmentExpr(method->localVarTable, expr);
            return;
        }
        case _ARRAY_EXPR: {
            attributeArrayCreatingExpr(method, expr);
            return;
        }
        case _FUNC_CALL:
        case _FUNC_ACCESS: {
            attributeFuncOrMethodCall(method, expr);
            return;
        }

        // TODO посмотреть, может что-то еще нужно проатрибутировать



    }
}

void ClassTable::attributeIdentifierExpr(LocalVariableTable *table, ExprNode* expr) {
    if (!table->items.contains(expr->identifierName)) {
        throw SemanticError::undefinedVariable(expr->identifierName);
    }

    LocalVariableTableElement* var = table->items[expr->identifierName];

    if (!var->isInitialized) {
        throw SemanticError::uninitializedVariable(expr->identifierName);
    }

    expr->semanticType = var->type;
}

void ClassTable::attributeAssignmentExpr(LocalVariableTable *table, ExprNode* expr) {
    ExprNode* left = expr->left;
    ExprNode* right = expr->right;

    if (!left || !right)
        return;

    // --- CASE: присваивание массиву ---
    if (left->type == _ARRAY_ACCESS) {
        // Проверяем, что тип слева массив
        if (!left->semanticType->isArray()) {
            throw SemanticError::invalidArrayAssignment(left->identifierName);
        }

        // Просто проверяем, что right можно присвоить elementType массива
        if (!left->semanticType->elementType->isReplaceable(*right->semanticType)) {
            throw SemanticError::assignmentTypeMismatch(
                left->semanticType->elementType->className,
                right->semanticType->className
            );
        }

        // Присваивание массива возвращает Unit
        expr->semanticType = SemanticType::classType("Unit");
        return;
    }

    // --- CASE: присваивание идентификатору ---
    if (left->type == _IDENTIFIER) {
        auto& vars = table->items;

        if (!vars.contains(left->identifierName)) {
            throw SemanticError::undefinedVariable(left->identifierName);
        }

        LocalVariableTableElement* var = vars[left->identifierName];

        // val нельзя переинициализировать
        if (var->isConst && var->isInitialized) {
            throw SemanticError::valReassignment(left->identifierName);
        }

        // Теперь переменная считается инициализированной
        var->isInitialized = true;

        // Проверка типов
        if (!left->semanticType->isReplaceable(*right->semanticType)) {
            throw SemanticError::assignmentTypeMismatch(
                left->semanticType->className,
                right->semanticType->className
            );
        }

        // Тип результата — Unit
        expr->semanticType = SemanticType::classType("Unit");
        return;
    }

    // --- CASE: недопустимый l-value ---
    throw SemanticError::unallowedAssignment();
}

void ClassTable::attributeArrayCreatingExpr(MethodTableElement* method, ExprNode* expr) {
    // expr->typeElements содержит TypeNode для типа элементов массива
    if (!expr->typeElements) {
        throw SemanticError::undefinedArrayElementType();
    }

    SemanticType* elementType = new SemanticType(expr->typeElements);

    // Если есть элементы (arrayOf(expr_list)), нужно проверить их типы
    if (expr->elements) {
        for (auto& elem : *expr->elements->exprs) {
            attributeExpression(method, elem, false); // рекурсивно атрибутируем элементы
            if (!elementType->isReplaceable(*elem->semanticType)) {
                throw SemanticError::assignmentTypeMismatch(elementType->className, elem->semanticType->className);
            }
        }
    }

    // Устанавливаем тип массива
    expr->semanticType = SemanticType::arrayType(elementType);
}

void ClassTable::attributeFuncOrMethodCall(MethodTableElement* currentMethod, ExprNode* expr) {
    std::vector<SemanticType*> paramTypes;

    // Атрибутируем параметры
    if (expr->params && expr->params->exprs) {
        for (ExprNode* param : *expr->params->exprs) {
            attributeExpression(currentMethod, param, false);

            if (!param->semanticType)
                throw SemanticError::nullSemanticType(param->type, param->identifierName);

            paramTypes.push_back(param->semanticType);
        }
    }

    // Формируем descriptor
    std::string paramDesc = ClassTableElement::addParamsToMethodDescriptor(paramTypes);

    // Определяем класс, относительно которого ищем метод
    std::string relatedClassName;
    if (expr->type == _FUNC_CALL) {
        relatedClassName = this->topLevelClassName;
    }
    else if (expr->type == _FUNC_ACCESS) {
        relatedClassName = expr->left->semanticType->className;
    }

    const std::string& methodName = expr->identifierName;
    if (!this->items.contains(relatedClassName))
        throw SemanticError::classNotFound(relatedClassName, methodName);

    auto& methods = this->items[relatedClassName]->methods->methods;

    bool isMethodFound = false;
    if (!methods.contains(methodName)) {
        // Если не нашли ни в классах, ни в свободных функциях, то ищем в стандартных
        for (auto& builtinClassName : builtinFunctionClasses) {
            relatedClassName = builtinClassName;
            methods = this->items[relatedClassName]->methods->methods;
            if (methods.contains(methodName)) {
                isMethodFound = true;
            }
        }
        if (!isMethodFound)
            throw SemanticError::methodNotFound(relatedClassName, methodName);
    }

    // TODO Вот тут могут найтись кандидаты из-за параметров по умолчанию
    if (!methods[methodName].contains(paramDesc))
        throw SemanticError::methodCandidateNotFound(relatedClassName, methodName, paramDesc);

    expr->semanticType = methods[methodName].find(paramDesc)->second->retType;
}


void ClassTable::initStdClasses() {
    ClassTable *classTable = new ClassTable(this->topLevelClassName);

    auto addClass = [&](const std::string& name, bool isOpen = false) {
        classTable->items[name] = new ClassTableElement();
        classTable->items[name]->clsName = name;
        classTable->items[name]->isOpen = isOpen ? 1 : 0;
        return classTable->items[name];
    };
    auto addMethod = [&](
        const std::string& nameClass,
        const std::string& nameMethod,
        SemanticType* returnType,
        const std::string& desk,
        const std::string& strDesc
    ) {
        classTable->items[nameClass]->methods->addMethod(
            nameMethod,
            desk,
            new MethodTableElement(
                0,
                0,
                nameMethod,
                strDesc,
                nullptr,
                returnType,
                {}
            )
        );
    };


    /* 1.0 Инициализация класса Int */
    addClass("JavaRTL/Int");
    /* 1.1 Конструктор */
    addMethod("JavaRTL/Int", "<init>", SemanticType::classType("JavaRTL/Unit"), "(I)", "(I)V");
    /* 1.2 Арифметические операции */
    addMethod("JavaRTL/Int", "plus", SemanticType::classType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "minus", SemanticType::classType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "times", SemanticType::classType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "div", SemanticType::classType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    /* 1.3 Унарные операции */
    addMethod("JavaRTL/Int", "uPlus", SemanticType::classType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "uMinus", SemanticType::classType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    /* 1.4 Массив */
    addMethod("JavaRTL/Int", "rangeTo", SemanticType::arrayType(SemanticType::classType("JavaRTL/Int")), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)[LJavaRTL/Int;");
    /* 1.5 Сравнение */
    addMethod("JavaRTL/Int", "greater", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "less", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "greaterEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "lessEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "equals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "notEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");

    /* 2.0 Инициализация класса Float */
    addClass("JavaRTL/Float");
    /* 2.1 Конструктор */
    addMethod("JavaRTL/Float", "<init>", SemanticType::classType("JavaRTL/Unit"), "(F)", "(F)V");
    /* 2.2 Арифметические операции */
    addMethod("JavaRTL/Float", "plus", SemanticType::classType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "minus", SemanticType::classType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "times", SemanticType::classType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "div", SemanticType::classType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    /* 2.3 Унарные операции */
    addMethod("JavaRTL/Float", "uPlus", SemanticType::classType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "uMinus", SemanticType::classType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Float", "greater", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "less", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "greaterEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "lessEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "equals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "notEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");


    /* 3.0 Инициализация класса Double */
    addClass("JavaRTL/Double");
    /* 3.1 Конструктор */
    addMethod("JavaRTL/Double", "<init>", SemanticType::classType("JavaRTL/Unit"), "(D)", "(D)V");
    /* 3.2 Арифметические операции */
    addMethod("JavaRTL/Double", "plus", SemanticType::classType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "minus", SemanticType::classType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "times", SemanticType::classType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "div", SemanticType::classType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    /* 3.3 Унарные операции */
    addMethod("JavaRTL/Double", "uPlus", SemanticType::classType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "uMinus", SemanticType::classType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Double", "greater", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "less", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "greaterEquals",SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "lessEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "equals",SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "notEquals",SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");

    /* 4. Инициализация класса String */
    addClass("JavaRTL/String");

    addMethod("JavaRTL/String", "toInt", SemanticType::classType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/String", "toFloat", SemanticType::classType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/String", "toDouble", SemanticType::classType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/String", "length", SemanticType::classType("JavaRTL/Int"), "()", "()I");

    /* 5. Инициализация класса Char */
    addClass("JavaRTL/Char");
    addMethod("JavaRTL/Char", "<init>", SemanticType::classType("JavaRTL/Unit"), "(C)", "(C)V");

    /* 6. Инициализация класса Boolean */
    addClass("JavaRTL/Boolean");
    addMethod("JavaRTL/Boolean", "<init>", SemanticType::classType("JavaRTL/Unit"), "(I)", "(I)V");  // true/false как 1/0
    addMethod("JavaRTL/Boolean", "not", SemanticType::classType("JavaRTL/Boolean"), "()", "()LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "and", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "or", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "equals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "notEquals", SemanticType::classType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");

    /* 7. Инициализация класса Any и Unit */
    addClass("JavaRTL/Any", true);
    addClass("JavaRTL/Unit");

    /* 8. I/O */
    addClass("JavaRTL/InputOutput");
    builtinFunctionClasses.push_back("JavaRTL/InputOutput");

    // print для всех типов
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // println для всех типов
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", SemanticType::classType("JavaRTL/Unit"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // readLine
    addMethod("JavaRTL/InputOutput", "readLine", SemanticType::classType("JavaRTL/String"), "()", "()LJavaRTL/String;");

    this->items = classTable->items;
}

bool ClassTable::isNeededType(const std::string& signature, const std::string& type) {
    size_t pos = type.rfind(')');
    if (pos == std::string::npos) return false;
    return type.substr(pos + 1) == signature;
}