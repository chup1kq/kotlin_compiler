#include "ClassTable.h"

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

class FuncParam;

ClassTable::ClassTable() {
    this->items = std::map<std::string, ClassTableElement*>();
}


void ClassTable::buildClassTable(KotlinFileNode* root, const std::string& fileName) {
    // Описываем все базовые классы
    items = ClassTable::initStdClasses()->items;

    // Создаем имя класса для top-level функций
    std::string topLevelClassName = makeTopLevelClassName(fileName);

    // Создаем базовый класс
    addBaseClass(topLevelClassName);

    // Добавляем методы все top level методы в базовый класс файла
    if (root->topLevelList->functionList)
        addTopLevelFunctionsToBaseClass(items[topLevelClassName], *root->topLevelList->functionList);

    // TODO дописать

}

void ClassTable::addBaseClass(const std::string &baseClassName) {
    // Cоздание класса для свободных функций
    ClassTableElement* topLevelFunctionsClass = new ClassTableElement();
    topLevelFunctionsClass->clsName = baseClassName;

    // Добавляем название и связываем его с классом
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

        // TODO дописать
        // vector<FuncParam*> params;
        // for (auto* p : *func->args->decls) {
        //     params.push_back(new FuncParam(p->varId, new SemanticType(p->varType)));
        // }


    }
}


ClassTable* ClassTable::initStdClasses() {
    ClassTable *classTable = new ClassTable();
    SemanticType * returnValue = nullptr;

    auto makeClassType = [&](const std::string& className) {
        SemanticType* t = new SemanticType();
        t->className = className;
        return t;
    };

    auto makeArrayType = [&](SemanticType* elementType) {
        SemanticType* t = new SemanticType();
        t->elementType = elementType;
        return t;
    };

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
        auto& methodMap = classTable->items[nameClass]->methods->methods;

        if (!methodMap.count(nameMethod))
            methodMap[nameMethod] = {};

        methodMap[nameMethod][desk] =
            new MethodTableElement(
                0,
                0,
                nameMethod,
                strDesc,
                nullptr,
                returnType,
                {}
            );
    };

    /* 1.0 Инициализация класса Int */
    addClass("JavaRTL/Int");
    /* 1.1 Конструктор */
    addMethod("JavaRTL/Int", "<init>", makeClassType("JavaRTL/Unit"), "(I)", "(I)V");
    /* 1.2 Арифметические операции */
    addMethod("JavaRTL/Int", "plus", makeClassType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "minus", makeClassType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "times", makeClassType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "div", makeClassType("JavaRTL/Int"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    /* 1.3 Унарные операции */
    addMethod("JavaRTL/Int", "uPlus", makeClassType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "uMinus", makeClassType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    /* 1.4 Массив */
    addMethod("JavaRTL/Int", "rangeTo", makeArrayType(makeClassType("JavaRTL/Int")), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)[LJavaRTL/Int;");
    /* 1.5 Сравнение */
    addMethod("JavaRTL/Int", "greater", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "less", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "greaterEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "lessEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "equals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "notEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");

    /* 2.0 Инициализация класса Float */
    addClass("JavaRTL/Float");
    /* 2.1 Конструктор */
    addMethod("JavaRTL/Float", "<init>", makeClassType("JavaRTL/Unit"), "(F)", "(F)V");
    /* 2.2 Арифметические операции */
    addMethod("JavaRTL/Float", "plus", makeClassType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "minus", makeClassType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "times", makeClassType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "div", makeClassType("JavaRTL/Float"), "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    /* 2.3 Унарные операции */
    addMethod("JavaRTL/Float", "uPlus", makeClassType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "uMinus", makeClassType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Float", "greater", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "less", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "greaterEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "lessEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "equals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "notEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");


    /* 3.0 Инициализация класса Double */
    addClass("JavaRTL/Double");
    /* 3.1 Конструктор */
    addMethod("JavaRTL/Double", "<init>", makeClassType("JavaRTL/Unit"), "(D)", "(D)V");
    /* 3.2 Арифметические операции */
    addMethod("JavaRTL/Double", "plus", makeClassType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "minus", makeClassType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "times", makeClassType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "div", makeClassType("JavaRTL/Double"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    /* 3.3 Унарные операции */
    addMethod("JavaRTL/Double", "uPlus", makeClassType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "uMinus", makeClassType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Double", "greater", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "less", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "greaterEquals",makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "lessEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "equals",makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "notEquals",makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");

    /* 4. Инициализация класса String */
    addClass("JavaRTL/String");

    addMethod("JavaRTL/String", "toInt", makeClassType("JavaRTL/Int"), "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/String", "toFloat", makeClassType("JavaRTL/Float"), "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/String", "toDouble", makeClassType("JavaRTL/Double"), "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/String", "length", makeClassType("JavaRTL/Int"), "()", "()I");

    /* 5. Инициализация класса Char */
    addClass("JavaRTL/Char");
    addMethod("JavaRTL/Char", "<init>", makeClassType("JavaRTL/Unit"), "(C)", "(C)V");

    /* 6. Инициализация класса Boolean */
    addClass("JavaRTL/Boolean");
    addMethod("JavaRTL/Boolean", "<init>", makeClassType("JavaRTL/Unit"), "(I)", "(I)V");  // true/false как 1/0
    addMethod("JavaRTL/Boolean", "not", makeClassType("JavaRTL/Boolean"), "()", "()LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "and", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "or", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "equals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "notEquals", makeClassType("JavaRTL/Boolean"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");

    /* 7. Инициализация класса Any и Unit */
    addClass("JavaRTL/Any", true);
    addClass("JavaRTL/Unit");

    /* 8. I/O */
    addClass("JavaRTL/InputOutput");

    // print для всех типов
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // println для всех типов
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", makeClassType("JavaRTL/Unit"), "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // readLine
    addMethod("JavaRTL/InputOutput", "readLine", makeClassType("JavaRTL/String"), "()", "()LJavaRTL/String;");

    return classTable;
}