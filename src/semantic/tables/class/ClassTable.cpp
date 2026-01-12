#include "ClassTable.h"

#include <iostream>

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

ClassTable::ClassTable(const std::string& fileName) {
    this->items.clear();
    this->topLevelClassName = makeTopLevelClassName(fileName);
}


void ClassTable::buildClassTable(KotlinFileNode* root) {
    initStdClasses();
    addBaseClass();

    // ✅ ОТЛАДКА:
    std::cout << "=== DEBUG topLevelList ===" << std::endl;
    std::cout << "root->topLevelList=" << (root->topLevelList ? "OK" : "NULL") << std::endl;
    std::cout << "root->topLevelList->functionList="
              << (root->topLevelList && root->topLevelList->functionList ? "OK" : "NULL") << std::endl;
    if (root->topLevelList && root->topLevelList->functionList) {
        std::cout << "functionList SIZE=" << root->topLevelList->functionList->size() << std::endl;
        for (auto* func : *root->topLevelList->functionList) {
            std::cout << "  → FUN: " << func->name << std::endl;
        }
    }

    // Добавляем методы все top level методы в базовый класс файла
    if (root->topLevelList && root->topLevelList->functionList) {
        std::cout << "CALLING addMethodsToTable..." << std::endl;
        items[topLevelClassName]->addMethodsToTable(*root->topLevelList->functionList);
    } else {
        std::cout << "❌ SKIPPED addMethodsToTable (functionList=NULL)" << std::endl;
    }

    // ✅ ПРОВЕРКА ДО attributeAndFillLocalsInClasses():
    std::cout << "MainKt methods BEFORE attribute:" << std::endl;
    if (items.find(topLevelClassName) != items.end()) {
        auto* mainKt = items[topLevelClassName];
        std::cout << "  Methods count: " << mainKt->methods->methods.size() << std::endl;
        for (auto& [name, overloads] : mainKt->methods->methods) {
            std::cout << "    → " << name << std::endl;
        }
    }

    // Добавляем все классы из файла в таблицу классов
    if (root->topLevelList->classList)
        addClassesToClassTable(items[topLevelClassName], *root->topLevelList->classList);

    std::cout << items.size() << endl;

    std::cout << "MainKt methods AFTER added all classes:" << std::endl;
    if (items.find(topLevelClassName) != items.end()) {
        auto* mainKt = items[topLevelClassName];
        std::cout << "  Methods count: " << mainKt->methods->methods.size() << std::endl;
        for (auto& [name, overloads] : mainKt->methods->methods) {
            std::cout << "    → " << name << std::endl;
        }
    }

    // Проверяем и заполняем локальные переменные в top level функциях
    attributeAndFillLocalsInClasses();

    std::cout << "MainKt methods AFTER ATTRIBUTE:" << std::endl;
    if (items.find(topLevelClassName) != items.end()) {
        auto* mainKt = items[topLevelClassName];
        std::cout << "  Methods count: " << mainKt->methods->methods.size() << std::endl;
        for (auto& [name, overloads] : mainKt->methods->methods) {
            std::cout << "    → " << name << std::endl;
        }
    }

    // TODO Доделать, строка не появляется
    for (auto& cls : this->items) {
        fillConstructorMethodRefs(cls.second);
    }

    std::cout << "=== STARTING fillLiterals ===" << std::endl;
    int classIdx = 0;
    for (auto& cls : this->items) {
        std::cout << "Class " << classIdx++ << ": " << cls.first << std::endl;
        if (cls.second->methods) {
            std::cout << "  Methods: " << cls.second->methods->methods.size() << std::endl;
        }
        fillLiterals(cls.second);
    }
    std::cout << "=== fillLiterals COMPLETED ===" << std::endl;

    std::cout << "=== STARTING fillMethodRefs ===" << std::endl;
    classIdx = 0;
    for (auto& cls : this->items) {
        std::cout << "Class " << classIdx++ << ": " << cls.first << std::endl;
        fillMethodRefs(cls.second);
    }
    std::cout << "=== fillMethodRefs COMPLETED ===" << std::endl;

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

    topLevelFunctionsClass->addDefaultConstructorIfNeeded();
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

        if (classNode->primaryConstructor) {
            newClass->addPrimaryConstructor(classNode->primaryConstructor);
        }
        newClass->addDefaultConstructorIfNeeded();

        // TODO заполнить поля

        newClass->addMethodsToTable(*classNode->body->methods);
    }
}

void ClassTable::attributeAndFillLocalsInClasses() {
    for (auto& [className, cls] : items) {
        for (auto& [methodName, overloads] : cls->methods->methods) {
            for (auto& [descriptor, method] : overloads) {
                std::cout << "Processing class=" << className
                          << " method=" << methodName
                          << " desc=" << descriptor << std::endl;

                if (!method) {
                    // std::cout << "  → skipped (method=nullptr)" << std::endl;
                    continue;
                }
                if (!method->start) {
                    // std::cout << "  → skipped (method->start=nullptr)" << std::endl;
                    continue;
                }
                if (!method->start->stmts) {
                    // std::cout << "  → skipped (method->start->stmts=nullptr)" << std::endl;
                    continue;
                }

                std::cout << "  → ATTRIBUTING (" << method->start->stmts->size() << " statements)" << std::endl;
                std::cout << "  → ATTRIBUTING (" << method->strName << " )" << std::endl;
                attributeAndFillLocals(method);
                std::cout << "  → ENDED ATTRIBUTING (" << method->strName << " )" << std::endl;
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
            attributeVarOrValStmt(method, stmt);
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
    const std::string& name = decl->varId;

    if (method->localVarTable->contains(name)) {
        throw SemanticError::redefinition(name);
    }

    SemanticType* currentType = new SemanticType(decl->varType);
    if (decl->defaultValue) {
        attributeExpression(method, decl->defaultValue);

        // Если тип явно не задан
        if (decl->varType->type == _UNDEFINED) {
            currentType = decl->defaultValue->semanticType;
        }

        SemanticType* exprType = decl->defaultValue->semanticType;
        // Проверяем, что указанный (:Int, ...) соответствует типу присваемого expr
        if (!currentType->isReplaceable(*exprType)) {
            throw SemanticError::notReplaceableTypesInStmtDeclaration(
                name,
                currentType->toString(),
                exprType->toString()
            );
        }
    }
    // Если нет дефолтного значения и тип не указан
    else if (decl->varType->type == _UNDEFINED) {
        throw SemanticError::undeclaredType(name);
    }

    bool isConst = (stmt->type == _VAL);
    bool isInitialized = (decl->defaultValue != nullptr);

    method->localVarTable->findOrAddLocalVar(
        name,
        currentType,
        isConst,
        isInitialized
    );
}

void ClassTable::attributeIfStmt(MethodTableElement *method, StmtNode *stmt) {
    attributeCondition(method, stmt->cond);

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
    attributeCondition(method, stmt->cond);

    if (stmt->blockStmts != nullptr) {
        for (auto* stmt : *stmt->blockStmts->stmts) {
            attributeAndFillLocalsInStatement(method, stmt);
        }
    }
}

void ClassTable::attributeCondition(MethodTableElement *method, ExprNode* cond) {
    if (!cond) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }

    attributeExpression(method, cond);

    if (!cond->semanticType) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }

    if (cond->semanticType->className != "JavaRTL/Boolean") {
        throw SemanticError::conditionNotBoolean(cond->semanticType->className);
    }
}


void ClassTable::attributeFor(MethodTableElement *method, StmtNode *stmt) {
    // TODO переделать на несколько переменных
    int varDeclCount = stmt->forIteratorList->decls->size();

    if (varDeclCount > 1) {
        throw SemanticError::multivarForLoop(method->strName + method->strDesc);
    }

    if (!stmt->cond)
        throw SemanticError::emptyRange();

    attributeExpression(method, stmt->cond);

    if (!stmt->cond->semanticType->isArray()) {
        throw SemanticError::forLoopNotArray(stmt->cond->semanticType->toString());
    }

    // Пока что только для одного проверяем
    VarDeclaration* iter = stmt->forIteratorList->decls->front();

    // TODO тут можно не выкидывать исключение, а проверять, что в таблице локальных переменных i имеет такой же тип, что и элементы массива
    if (method->localVarTable->contains(iter->varId)) {
        throw SemanticError::redefinition(iter->varId);
    }

    if (iter->varType->customName != "") {
        // Тип итераратора равен типу элементов перебираемого массива
        if (iter->varType->customName != stmt->cond->semanticType->elementType->className) {
            throw SemanticError::invalidIteratorType(
                iter->varType->customName,
                stmt->cond->semanticType->elementType->toString()
            );
        }
    }
    else {
        // тут просто присваиваем, не учитываем вложенные массивы
        iter->varType->customName = stmt->cond->semanticType->elementType->className;
        std::cout << iter->varType->customName << std::endl;
    }

    // Ставим итератору массива первое значение из перебираемого списка и добавляем в таблицу локальных переменных
    // iter->defaultValue = stmt->cond->elements->exprs->front(); // тут nullptr выкидывает
    method->localVarTable->findOrAddLocalVar(iter->varId, stmt->cond->semanticType->elementType, false, true);

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

    if (stmt->expr == nullptr) {
        // Если return без expr, а возвращаемое значение функции не void
        if (retType->className != "Unit") {
            throw SemanticError::missingReturnValue(method->strName + method->strDesc);
        }
        return;
    }

    attributeExpression(method, stmt->expr);

    // return expr;
    if (retType->className == "Unit" && stmt->expr->semanticType->className != "Unit") {
        throw SemanticError::returnFromVoid(method->strName + method->strDesc);
    }

    if (retType->className != stmt->expr->semanticType->className) {
        throw SemanticError::returnTypeMismatch(method->strName + method->strDesc);
    }
}

void ClassTable::attributeExpression(MethodTableElement *method, ExprNode *expr, bool isStatementContext) {
    std::cout << "entered ClassTable::attributeExpression" << std::endl;
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
        case _INTEGER_LITERAL: {
            expr->fromLiteral = _FROM_INT;
            std::cout << "Int value = " + to_string(expr->intValue) << std::endl;
            std::cout << "FromLiteral = " + to_string(expr->fromLiteral) << std::endl;
            expr->semanticType = SemanticType::classType("JavaRTL/Int");
            return;
        }
        case _FLOAT_LITERAL: {
            expr->semanticType = SemanticType::classType("JavaRTL/Float");
            return;
        }
        case _DOUBLE_LITERAL: {
            expr->semanticType = SemanticType::classType("JavaRTL/Double");
            return;
        }
        case _STRING_LITERAL: {
            expr->semanticType = SemanticType::classType("JavaRTL/String");
            return;
        }
        case _CHAR_LITERAL: {
            expr->semanticType = SemanticType::classType("JavaRTL/Char");
            return;
        }
        case _BOOL_LITERAL: {
            expr->semanticType = SemanticType::classType("JavaRTL/Boolean");
            return;
        }
        case _BRACKETS: {
            expr->semanticType = leftType;
            return;
        }
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
    if (expr->typeElements) {
        expr->semanticType = SemanticType::arrayType(
            new SemanticType(expr->typeElements)
        );
    }

    // Аттрибутируем элементы внутри скобок
    if (expr->elements && expr->elements->exprs && !expr->elements->exprs->empty()) {
        for (auto& e : *expr->elements->exprs) {
            attributeExpression(method, e);
        }

        // Получаем тип элементов (все должны быть одинаковыми)
        SemanticType elementsType = checkSameElementsType(expr->elements->exprs);

        // Если есть заданный тип массива (arrayOf<Int?>), проверяем совместимость
        if (expr->semanticType) {
            SemanticType* expectedElementType = expr->semanticType->elementType;
            if (!expectedElementType->isReplaceable(elementsType)) {
                throw SemanticError::notReplaceableTypesInArrayCreation(
                    expectedElementType->toString(),
                    elementsType.toString()
                );
            }
        } else {
            // Тип не задан -> выводим из элементов
            expr->semanticType = SemanticType::arrayType(new SemanticType(elementsType));
        }
    } else {
        // Пустой arrayOf<> или arrayOf() - используем заданный тип или undefined
        if (!expr->semanticType) {
            expr->semanticType = SemanticType::arrayType(
                new SemanticType(TypeNode::createUndefinedType())
            );
        }
    }
}

SemanticType ClassTable::checkSameElementsType(std::list<ExprNode*> *exprs) {
    if (!exprs || exprs->empty()) {
        return SemanticType(TypeNode::createUndefinedType());
    }

    SemanticType firstType = *exprs->front()->semanticType;

    for (auto& expr : *exprs) {
        if (!firstType.isReplaceable(*expr->semanticType)) {
            throw SemanticError::differentTypesInArrayCreation(
                firstType.className,
                expr->semanticType->className
            );
        }
    }

    return firstType;
}


void ClassTable::attributeFuncOrMethodCall(MethodTableElement* currentMethod, ExprNode* expr) {
    std::cout << "Entered ClassTable::attributeFuncOrMethodCall" << std::endl;
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

    // "(LJavaRTL/String;)"
    std::string paramDesc = ClassTableElement::addParamsToMethodDescriptor(paramTypes);

    // Определяем класс
    std::string relatedClassName;
    if (expr->type == _FUNC_CALL) {
        relatedClassName = this->topLevelClassName;
    } else if (expr->type == _FUNC_ACCESS) {
        relatedClassName = expr->left->semanticType->className;
    }

    const std::string& methodName = expr->identifierName;
    if (!this->items.contains(relatedClassName))
        throw SemanticError::classNotFound(relatedClassName, methodName);

    auto& methodsMap = this->items[relatedClassName]->methods->methods;

    bool isMethodFoundInClass = methodsMap.contains(methodName);
    if (!isMethodFoundInClass) {
        // Поиск в builtin классах
        bool foundInBuiltin = false;
        for (auto& builtinClassName : builtinFunctionClasses) {
            if (!this->items.contains(builtinClassName))
                continue;
            auto& m = this->items[builtinClassName]->methods->methods;
            if (m.contains(methodName)) {
                relatedClassName = builtinClassName;
                methodsMap = m;
                foundInBuiltin = true;
                break;
            }
        }
        if (!foundInBuiltin)
            throw SemanticError::methodNotFound(relatedClassName, methodName);
    }

    auto& overloads = methodsMap[methodName];

    // Ищем перегрузку по prefix match полного дескриптора
    MethodTableElement* chosen = nullptr;
    std::string chosenDesc;

    for (auto& [fullDesc, methodElem] : overloads) {
        // fullDesc: "(LJavaRTL/String;)LJavaRTL/Unit;"
        if (fullDesc.rfind(paramDesc, 0) == 0) { // начинается с paramDesc
            chosen = methodElem;
            chosenDesc = fullDesc;
            break;
        }
    }

    if (!chosen) {
        std::cout << "paramDesc: " << paramDesc << std::endl;
        for (auto& [fullDesc, _] : overloads) {
            std::cout << "candidate: " << fullDesc << std::endl;
        }
        throw SemanticError::methodCandidateNotFound(relatedClassName, methodName, paramDesc);
    }

    expr->semanticType = chosen->retType;
}

void ClassTable::fillLiterals(ClassTableElement *elem) {
    std::cout << "Entered fillLiterals::" << elem->clsName << std::endl;
    if (!elem || !elem->methods || !elem->methods->methods.empty() == false) {
        std::cout << "  → SKIPPED (no methods)" << std::endl;
        return;
    }

    for (auto& [methodName, overloads] : elem->methods->methods) {
        std::cout << "  → Scanning method: " << methodName << std::endl;

        for (auto& [desc, method] : overloads) {
            std::cout << "    → Checking '" << methodName << "' (" << desc << ")" << std::endl;
            std::cout << "      method=" << (method ? "OK" : "NULL") << std::endl;
            std::cout << "      method->start=" << (method && method->start ? "OK" : "NULL") << std::endl;
            std::cout << "      method->start->stmts=" << (method && method->start && method->start->stmts ? "OK" : "NULL") << std::endl;

            if (!method || !method->start || !method->start->stmts) {
                std::cout << "      → SKIPPED (missing start/stmts)" << std::endl;
                continue;
            }

            std::cout << "Filling " << methodName << " (" << method->start->stmts->size() << " statements)" << std::endl;
            for (auto& stmt : *method->start->stmts) {
                fillLiteralsInStatement(stmt, elem);
            }
        }
    }
}



void ClassTable::fillMethodRefs(ClassTableElement *elem) {
    if (!elem || !elem->methods || !elem->methods->methods.size())
        return;

    std::cout << "  → Processing method refs" << std::endl;

    for (auto& [methodName, overloads] : elem->methods->methods) {
        for (auto& [desc, method] : overloads) {
            if (!method || !method->start || !method->start->stmts)
                continue;

            for (auto& stmt : *method->start->stmts) {
                fillMethodRefsInStatement(stmt, elem);
            }
        }
    }
}


void ClassTable::fillMethodRefsInStatement(StmtNode *stmt, ClassTableElement *elem) {
    if (!stmt) return;

    switch (stmt->type) {
        case _EXPRESSION:
            if (stmt->expr && stmt->expr->semanticType)
                fillMethodRefsInExpression(stmt->expr, elem);
            break;
        case _RETURN:
            if (stmt->expr && stmt->expr->semanticType)
                fillMethodRefsInExpression(stmt->expr, elem);
            break;
        case _VAL:
        case _VAR:
            if (stmt->varDeclaration && stmt->varDeclaration->defaultValue
                && stmt->varDeclaration->defaultValue->semanticType)
                fillMethodRefsInExpression(stmt->varDeclaration->defaultValue, elem);
            break;
        case _DO_WHILE:
        case _WHILE:
            if (stmt->cond && stmt->cond->semanticType)
                fillMethodRefsInExpression(stmt->cond, elem);
            if (stmt->blockStmts && stmt->blockStmts->stmts) {
                for (auto& s : *stmt->blockStmts->stmts) {
                    fillMethodRefsInStatement(s, elem);
                }
            }
            break;
        case _FOR:
            if (stmt->cond && stmt->cond->semanticType)
                fillMethodRefsInExpression(stmt->cond, elem);
            if (stmt->blockStmts && stmt->blockStmts->stmts) {
                for (auto& s : *stmt->blockStmts->stmts) {
                    fillMethodRefsInStatement(s, elem);
                }
            }
            break;
        case _IF_STMT:
            if (stmt->cond && stmt->cond->semanticType)
                fillMethodRefsInExpression(stmt->cond, elem);
            if (stmt->trueStmtList && stmt->trueStmtList->stmts) {
                for (auto& s : *stmt->trueStmtList->stmts) {
                    fillMethodRefsInStatement(s, elem);
                }
            }
            if (stmt->falseStmtList && stmt->falseStmtList->stmts) {
                for (auto& s : *stmt->falseStmtList->stmts) {
                    fillMethodRefsInStatement(s, elem);
                }
            }
            break;
    }
}


/* TODO ? */
void ClassTable::fillMethodRefsInExpression(ExprNode *expr, ClassTableElement *elem) {
    if (!expr || !expr->semanticType) return;

    // ✅ РЕКУРСИЯ - сначала дочерние узлы
    if (expr->left && expr->left->semanticType)
        fillMethodRefsInExpression(expr->left, elem);
    if (expr->right && expr->right->semanticType)
        fillMethodRefsInExpression(expr->right, elem);

    // ✅ Параметры
    if (expr->params && expr->params->exprs) {
        for (auto& param : *expr->params->exprs) {
            if (param && param->semanticType)
                fillMethodRefsInExpression(param, elem);
        }
    }

    // ✅ Элементы массива
    if (expr->elements && expr->elements->exprs) {
        for (auto& e : *expr->elements->exprs) {
            if (e && e->semanticType)
                fillMethodRefsInExpression(e, elem);
        }
    }

    // ✅ FUNC_CALL / FUNC_ACCESS
    if (expr->type != _FUNC_CALL && expr->type != _FUNC_ACCESS)
        return;

    if (expr->identifierName.empty()) return;

    // Строим дескриптор "()LJavaRTL/Unit;"
    std::string paramDesc = "(";
    if (expr->params && expr->params->exprs) {
        for (auto& param : *expr->params->exprs) {
            if (param && param->semanticType) {
                paramDesc += getTypeDescriptor(param->semanticType);
            }
        }
    }
    paramDesc += ")";

    if (expr->semanticType) {
        paramDesc += getTypeDescriptor(expr->semanticType);
    } else {
        paramDesc += "V";  // void
    }

    // Создаем CONSTANT_Methodref
    int nameIdx = elem->constants->findOrAddConstant(UTF8, expr->identifierName);
    int descIdx = elem->constants->findOrAddConstant(UTF8, paramDesc);
    int nameAndType = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, nameIdx, descIdx);

    if (expr->type == _FUNC_CALL) {
        // println -> JavaRTL/InputOutput.println
        if (expr->identifierName == "print" || expr->identifierName == "println" || expr->identifierName == "readLine") {
            int ioClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/InputOutput");
            int ioClass = elem->constants->findOrAddConstant(Class, "", 0, 0, ioClassName);
            elem->constants->findOrAddConstant(MethodRef, "", 0, 0, ioClass, nameAndType);
        } else {
            // Обычный вызов метода текущего класса
            int thisClassName = elem->constants->findOrAddConstant(UTF8, elem->clsName);
            int thisClass = elem->constants->findOrAddConstant(Class, "", 0, 0, thisClassName);
            elem->constants->findOrAddConstant(MethodRef, "", 0, 0, thisClass, nameAndType);
        }
    } else if (expr->type == _FUNC_ACCESS) {
        // Метод объекта: obj.method()
        if (expr->left && expr->left->semanticType) {
            std::string className = expr->left->semanticType->className;
            int clsNameIdx = elem->constants->findOrAddConstant(UTF8, className);
            int clsIdx = elem->constants->findOrAddConstant(Class, "", 0, 0, clsNameIdx);
            elem->constants->findOrAddConstant(MethodRef, "", 0, 0, clsIdx, nameAndType);
        }
    }
}


void ClassTable::fillLiteralsInStatement(StmtNode *stmt, ClassTableElement *elem) {
    std:cout << "Entered fillLiteralsInExpression Semantic type = " << stmt->semanticType->toString() << endl;
    if (stmt->type == _EXPRESSION) {
        fillLiteralsInExpression(stmt->expr, elem);
    } else if (stmt->type == _RETURN) {
        fillLiteralsInExpression(stmt->expr, elem);
    } else if (stmt->type == _VAL || stmt->type == _VAR) {
        fillLiteralsInExpression(stmt->expr, elem);
    } else if (stmt->type ==_DO_WHILE || stmt->type == _WHILE || stmt->type == _FOR) {
        fillLiteralsInExpression(stmt->cond, elem);
        for (auto & stmt : *stmt->blockStmts->stmts) {
            fillLiteralsInStatement(stmt, elem);
        }
    }
}

void ClassTable::fillLiteralsInExpression(ExprNode *expr, ClassTableElement *elem) {
    std:cout << "Entered fillLiteralsInExpression Semantic type = " << expr->semanticType->toString() << endl;
    if (!expr || !expr->semanticType) return;

    if (expr->fromLiteral == _FROM_BOOLEAN) {
        // FieldRef: JavaRTL/Boolean._ivalue:I + MethodRef <init>(I)V
        int fieldType = elem->constants->findOrAddConstant(UTF8, "I");
        int fieldName = elem->constants->findOrAddConstant(UTF8, "_ivalue");
        int fieldNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, fieldName, fieldType);

        int boolClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Boolean");
        int boolClass = elem->constants->findOrAddConstant(Class, "", 0, 0, boolClassName);
        elem->constants->findOrAddConstant(FieldRef, "", 0, 0, boolClass, fieldNAT);  // поле

        // MethodRef
        int initName = elem->constants->findOrAddConstant(UTF8, "<init>");
        int initDesc = elem->constants->findOrAddConstant(UTF8, "(I)V");
        int initNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, initName, initDesc);
        elem->constants->findOrAddConstant(MethodRef, "", 0, 0, boolClass, initNAT);

        elem->constants->findOrAddConstant(Integer, "", expr->intValue ? 1 : 0, 0, 0);

    } else if (expr->fromLiteral == _FROM_INT) {
        elem->constants->findOrAddConstant(Integer, "", expr->intValue, 0, 0);

        // FieldRef + MethodRef (как в эталоне)
        int fieldType = elem->constants->findOrAddConstant(UTF8, "I");
        int fieldName = elem->constants->findOrAddConstant(UTF8, "_value");
        int fieldNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, fieldName, fieldType);

        int intClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Int");
        int intClass = elem->constants->findOrAddConstant(Class, "", 0, 0, intClassName);
        elem->constants->findOrAddConstant(FieldRef, "", 0, 0, intClass, fieldNAT);

        int initName = elem->constants->findOrAddConstant(UTF8, "<init>");
        int initDesc = elem->constants->findOrAddConstant(UTF8, "(I)V");
        int initNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, initName, initDesc);
        elem->constants->findOrAddConstant(MethodRef, "", 0, 0, intClass, initNAT);

    } else if (expr->fromLiteral == _FROM_CHAR) {
        // Integer + MethodRef
        elem->constants->findOrAddConstant(Integer, "", expr->intValue, 0, 0);  // код символа

        int charClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Char");
        int charClass = elem->constants->findOrAddConstant(Class, "", 0, 0, charClassName);
        int initName = elem->constants->findOrAddConstant(UTF8, "<init>");
        int initDesc = elem->constants->findOrAddConstant(UTF8, "(C)V");
        int initNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, initName, initDesc);
        elem->constants->findOrAddConstant(MethodRef, "", 0, 0, charClass, initNAT);

    } else if (expr->fromLiteral == _FROM_STRING) {
        // String + MethodRef
        int strUtf8 = elem->constants->findOrAddConstant(UTF8, expr->stringValue);
        elem->constants->findOrAddConstant(String, "", 0, 0, strUtf8);

        int strClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/String");
        int strClass = elem->constants->findOrAddConstant(Class, "", 0, 0, strClassName);
        int initName = elem->constants->findOrAddConstant(UTF8, "<init>");
        int initDesc = elem->constants->findOrAddConstant(UTF8, "(Ljava/lang/String;)V");
        int initNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, initName, initDesc);
        elem->constants->findOrAddConstant(MethodRef, "", 0, 0, strClass, initNAT);

    } else if (expr->fromLiteral == _FROM_UNIT) {
        // Только MethodRef
        int unitClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Unit");
        int unitClass = elem->constants->findOrAddConstant(Class, "", 0, 0, unitClassName);
        int initName = elem->constants->findOrAddConstant(UTF8, "<init>");
        int initDesc = elem->constants->findOrAddConstant(UTF8, "()V");
        int initNAT = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, initName, initDesc);
        elem->constants->findOrAddConstant(MethodRef, "", 0, 0, unitClass, initNAT);
    }

    // Рекурсия (ваш код уже правильный)
    if (expr->left && expr->left->semanticType)
        fillLiteralsInExpression(expr->left, elem);
    if (expr->right && expr->right->semanticType)
        fillLiteralsInExpression(expr->right, elem);
    if (expr->params && expr->params->exprs) {
        for (auto& param : *expr->params->exprs) {
            if (param && param->semanticType) fillLiteralsInExpression(param, elem);
        }
    }
}

/* TODO ? */
void ClassTable::fillFieldConstants(ClassTableElement *elem) {
    // Boolean._ivalue:I
    int boolClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Boolean");
    int boolClass = elem->constants->findOrAddConstant(Class, "", 0, 0, boolClassName);

    int ivalueName = elem->constants->findOrAddConstant(UTF8, "_ivalue");
    int intType = elem->constants->findOrAddConstant(UTF8, "I");
    int boolNameAndType = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, ivalueName, intType);
    elem->constants->findOrAddConstant(FieldRef, "", 0, 0, boolClass, boolNameAndType);  // сохраняем!

    // Int._value:I (переиспользуем "I")
    int intClassName = elem->constants->findOrAddConstant(UTF8, "JavaRTL/Int");
    int intClass = elem->constants->findOrAddConstant(Class, "", 0, 0, intClassName);

    int valueName = elem->constants->findOrAddConstant(UTF8, "_value");
    int intNameAndType = elem->constants->findOrAddConstant(NameAndType, "", 0, 0, valueName, intType);  // тот же intType!
    elem->constants->findOrAddConstant(FieldRef, "", 0, 0, intClass, intNameAndType);
}

void ClassTable::fillConstructorMethodRefs(ClassTableElement* cls) {
    // Для каждого конструктора
    auto& ctors = cls->methods->methods["<init>"];
    for (auto& [desc, method] : ctors) {
        // Добавляем вызов super.<init>()
        int initNameIdx = cls->constants->findOrAddConstant(UTF8, "<init>");
        int initDescIdx = cls->constants->findOrAddConstant(UTF8, "()V");

        // ✅ #8 = NameAndType #6:#7
        int nameAndTypeIdx = cls->constants->findOrAddConstant(NameAndType, "", 0, 0, initNameIdx, initDescIdx);

        // ✅ #9 = Methodref Object:#8
        int objectClassIdx = cls->constants->findOrAddConstant(Class, "", 0, 0,
            cls->constants->findOrAddConstant(UTF8, "java/lang/Object"));
        int superInitRef = cls->constants->findOrAddConstant(MethodRef, "", 0, 0, objectClassIdx, nameAndTypeIdx);

        // Сохраняем для генерации байткода
        method->superConstructorCall = superInitRef;
    }
}


void ClassTable::initStdClasses() {
    auto addClass = [&](const std::string& name, bool isOpen = false) {
        if (this->items.contains(name)) return this->items[name];
        ClassTableElement* cls = new ClassTableElement();
        cls->clsName = name;
        cls->isOpen = isOpen;
        this->items[name] = cls;
        return cls;
    };

    auto addMethod = [&](const std::string& nameClass,
                     const std::string& nameMethod,
                     SemanticType* returnType,
                     const std::string& paramsDesc,   // "(LJavaRTL/String;)"
                     const std::string& fullDesc) {   // "(LJavaRTL/String;)LJavaRTL/Unit;"
        auto& methodsMap = this->items[nameClass]->methods->methods;

        if (!methodsMap.contains(nameMethod)) {
            methodsMap[nameMethod] = {};
        }

        // Ключ — ПОЛНЫЙ дескриптор
        methodsMap[nameMethod][fullDesc] =
            new MethodTableElement(
                0,            // methodNameNumber (если нужно — потом заполните из ConstantTable)
                0,            // methodDescNumber
                nameMethod,   // strName
                fullDesc,     // strDesc = полный дескриптор
                nullptr,      // start
                returnType,   // retType
                {}            // params
            );
    };



    /* 1.0 Инициализация класса Int */
    addClass("JavaRTL/Int");
    builtinFunctionClasses.push_back("JavaRTL/Int");
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
    builtinFunctionClasses.push_back("JavaRTL/Float");
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
    builtinFunctionClasses.push_back("JavaRTL/Double");
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
    builtinFunctionClasses.push_back("JavaRTL/String");

    addMethod("JavaRTL/String", "toInt", SemanticType::classType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/String", "toFloat", SemanticType::classType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/String", "toDouble", SemanticType::classType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/String", "length", SemanticType::classType("JavaRTL/Int"), "()", "()I");

    /* 5. Инициализация класса Char */
    addClass("JavaRTL/Char");
    builtinFunctionClasses.push_back("JavaRTL/Char");
    addMethod("JavaRTL/Char", "<init>", SemanticType::classType("JavaRTL/Unit"), "(C)", "(C)V");

    /* 6. Инициализация класса Boolean */
    addClass("JavaRTL/Boolean");
    builtinFunctionClasses.push_back("JavaRTL/Boolean");
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

}

bool ClassTable::isNeededType(const std::string& signature, const std::string& type) {
    size_t pos = type.rfind(')');
    if (pos == std::string::npos) return false;
    return type.substr(pos + 1) == signature;
}

std::string ClassTable::getTypeDescriptor(SemanticType* type) {
    if (!type) return "Ljava/lang/Object;";

    /* TODO */
    if (type->className == "Int") {
        return "LJavaRTL/Int;";
    } else if (type->className == "Boolean") {
        return "LJavaRTL/Boolean;";
    } else if (type->className == "Float") {
        return "LJavaRTL/Float;";
    } else if (type->className == "Double") {
        return "LJavaRTL/Double;";
    } else if (type->className == "String") {
        return "LJavaRTL/String;";
    } else if (type->className == "Char") {
        return "LJavaRTL/Char;";
    } else {
        return "L" + type->className + ";"; // "Ljava/lang/Object;" мб
    }
}