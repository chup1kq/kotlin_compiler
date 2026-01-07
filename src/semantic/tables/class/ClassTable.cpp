#include "ClassTable.h"

#include <iostream>

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

class FuncParam;

ClassTable::ClassTable() {
    this->items = std::map<std::string, ClassTableElement*>();
}


void ClassTable::buildClassTable(KotlinFileNode* root, const std::string& fileName) {
    // Описываем все базовые классы
    initStdClasses();

    // Создаем имя класса для top-level функций
    std::string topLevelClassName = makeTopLevelClassName(fileName);

    // Создаем базовый класс
    addBaseClass(topLevelClassName);

    // Добавляем методы все top level методы в базовый класс файла
    if (root->topLevelList->functionList)
        addTopLevelFunctionsToBaseClass(items[topLevelClassName], *root->topLevelList->functionList);

    // Добавляем все классы из файла в таблицу классов
    if (root->topLevelList->classList)
        addClassesToClassTable(items[topLevelClassName], *root->topLevelList->classList);


    // TODO дописать
}

void ClassTable::addBaseClass(const std::string &baseClassName) {
    // Cоздание класса для свободных функций
    ClassTableElement* topLevelFunctionsClass = new ClassTableElement();
    topLevelFunctionsClass->clsName = baseClassName;

    // Добавляем название и связываем его с классом
    // TODO вот это дублируется с методом addClassesToClassTable
    int topLevelUtf8 = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, baseClassName);
    int cls = topLevelFunctionsClass->constants->findOrAddConstant(Class, "" ,0, 0, topLevelUtf8);

    topLevelFunctionsClass->name = topLevelUtf8;
    topLevelFunctionsClass->thisClass = cls;

    // Добавляем название родителя и связываем его с классом
    int parent = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, "java/lang/Object");
    int parentClass = topLevelFunctionsClass->constants->findOrAddConstant(Class, "", 0, 0, parent);

    topLevelFunctionsClass->superName = parent;
    topLevelFunctionsClass->superClass = parentClass;

    // Добавляем базовый класс в таблицу
    items[baseClassName] = topLevelFunctionsClass;
}


bool ClassTable::exists(const std::string& name) {
    return items.find(name) != items.end();
}


void ClassTable::add(const std::string& name, ClassTableElement* elem) {
    items[name] = elem;
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

void ClassTable::addTopLevelFunctionsToBaseClass(ClassTableElement *baseClass, std::list<FunNode *> funcList) {
    for (auto& func : funcList) {
        // Имя  метода
        std::string ident = func->name;

        // Возвращаемое значение
        SemanticType* retVal = new SemanticType(func->type);

        // Параметры метода
        vector<FuncParam*> params;
        if (func->args && func->args->decls) {
            for (auto* arg : *func->args->decls) {
                if (!arg)
                    continue;

                params.push_back(new FuncParam(arg->varId, new SemanticType(arg->varType)));
            }
        }

        std::string descriptor = createMethodDescriptor(params, retVal);

        if (baseClass->methods->methods.contains(ident)) {
            if (baseClass->methods->methods[ident].contains(descriptor) ) {
                throw SemanticError::methodAlreadyExists(ident);
            }

            int methodName = baseClass->constants->findOrAddConstant(UTF8, ident);
            int methodDesc = baseClass->constants->findOrAddConstant(UTF8, descriptor);

            baseClass->methods->methods.find(ident)->second[descriptor] = new MethodTableElement(methodName, methodDesc, ident, descriptor, func->body, retVal, params);
            // тут еще он добавляет в FunctionTable
        }
        else {
            int methodName = baseClass->constants->findOrAddConstant(UTF8, ident);
            int methodDesc = baseClass->constants->findOrAddConstant(UTF8, descriptor);

            baseClass->methods->methods[ident] = std::map<std::string, MethodTableElement*>();
            baseClass->methods->methods.find(ident)->second[descriptor] = new MethodTableElement(methodName, methodDesc, ident, descriptor, func->body, retVal, params);
            // тут еще он добавляет в FunctionTable
        }

        for (auto& param : params) {
            // тут еще он добавляет в FunctionTable
            baseClass->methods->methods.find(ident)->second[descriptor]->localVarTable->findOrAddLocalVar("", param->type, 1, 1);
        }

    }
}

std::string ClassTable::createMethodDescriptor(vector<FuncParam*> params, SemanticType* returnType) {
    std::string desc = addParamsToMethodDescriptor(params);

    if (!returnType->isArray())
        desc += "L";
    else
        desc += "[L";

    desc += returnType->className;
    desc += ";";

    // TODO удалить cout
    std::cout << desc << std::endl;
    return desc;
}

std::string ClassTable::addParamsToMethodDescriptor(vector<FuncParam *> params) {
    std::string desc = "(";

    for (auto* param : params) {
        if (!param->type->isArray())
            desc += "L";
        else
            desc += "[L";

        desc += param->type->className;
        desc += ";";
    }
    desc += ")";

    return desc;
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
        addPrimaryConstructor(newClass, classNode);
        // TODO заполнить поля
        // TODO заполнить методы
    }
}

void ClassTable::addPrimaryConstructor(ClassTableElement* cls, ClassNode *classNode) {
    std::string methodName = "<init>"; // Имя конструктора для класса
    std::string descriptor;

    SemanticType* retVal = SemanticType::classType(cls->clsName);

    // TODO вот тут есть дублирование с методом addTopLevelFunctionsToBaseClass, посмотреть, может что-то вынести
    std::vector<FuncParam*> params;
    if (classNode->primaryConstructor) {
        if (classNode->primaryConstructor->args) {
            for (auto* arg : classNode->primaryConstructor->args->args) {
                std::string ident;
                SemanticType* type;

                if (auto* v = dynamic_cast<VarDeclaration*>(arg)) {
                    ident = v->varId;
                    type = new SemanticType(v->varType);
                }

                else if (auto* s = dynamic_cast<StmtNode*>(arg)) {
                    // TODO если был передан параметр как поле, то его нужно добавить к полям
                }

                params.push_back(new FuncParam(ident, type));
            }
        }
        descriptor = addParamsToMethodDescriptor(params);

        std::string descKey = descriptor;
        descriptor += "V";

        if (cls->methods->methods.contains(methodName)) {
            if (cls->methods->methods[methodName].contains(descKey) ) {
                throw SemanticError::constructorAlreadyExists(descKey);
            }

            int name = cls->constants->findOrAddConstant(UTF8, methodName);
            int desc = cls->constants->findOrAddConstant(UTF8, descriptor);

            cls->methods->methods.find(methodName)->second[descriptor] = new MethodTableElement(name, desc, methodName, descriptor, classNode->primaryConstructor->stmts, retVal, params);
        }
        else {
            int name = cls->constants->findOrAddConstant(UTF8, methodName);
            int desc = cls->constants->findOrAddConstant(UTF8, descriptor);

            cls->methods->methods[methodName] = std::map<std::string, MethodTableElement*>();
            cls->methods->methods.find(methodName)->second[descriptor] = new MethodTableElement(name, desc, methodName, descriptor, classNode->primaryConstructor->stmts, retVal, params);
        }
    }
    // TODO доделать
}


void ClassTable::initStdClasses() {
    ClassTable *classTable = new ClassTable();

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