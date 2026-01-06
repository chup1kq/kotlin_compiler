#include "ClassTable.h"

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

class FuncParam;

ClassTable::ClassTable() {
    this->items = std::map<std::string, ClassTableElement*>();
}


void ClassTable::buildClassTable(KotlinFileNode* root, const std::string& fileName) {

    // имя класса для top-level функций
    std::string topLevelClassName = makeTopLevelClassName(fileName);

    // создание класса для свободных функций
    ClassTableElement* topLevelFunctionsClass = new ClassTableElement();
    topLevelFunctionsClass->clsName = topLevelClassName;

    int topLevelUtf8 = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, topLevelClassName);
    int cls = topLevelFunctionsClass->constants->findOrAddConstant(Class, "" ,0, 0, topLevelUtf8);

    topLevelFunctionsClass->name = topLevelUtf8;
    topLevelFunctionsClass->thisClass = cls;

    int parent = topLevelFunctionsClass->constants->findOrAddConstant(UTF8, "java/lang/Object");
    int parentClass = topLevelFunctionsClass->constants->findOrAddConstant(Class, "", 0, 0, parent);

    topLevelFunctionsClass->superName = parent;
    topLevelFunctionsClass->superClass = parentClass;

    this->items[topLevelClassName] = topLevelFunctionsClass;

    // TODO дописать

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

ClassTable ClassTable::initStdClasses() {
    ClassTable *classTable = new ClassTable();
    TypeTable * returnValue = nullptr;

    auto addClass = [&](const std::string& name, bool isOpen = false) {
        classTable->items[name] = new ClassTableElement();
        classTable->items[name]->clsName = name;
        classTable->items[name]->isOpen = isOpen ? 1 : 0;
        return classTable->items[name];
    };
    auto addMethod = [&](
        const std::string& nameClass,
        const std::string& nameMethod,
        TypeNodeType type,
        const std::string& desk,
        std::string strDesc){
        classTable->items[nameClass]->methods->methods[nameMethod] =
            std::map<std::string, class MethodTableElement*>();
        returnValue = new TypeTable();
        returnValue->className = classTable->items[nameClass]->clsName;
        returnValue->type = type;
        classTable->items[nameClass]->methods->methods[nameMethod][desk] =
            new MethodTableElement(
                0,
                0,
                nameMethod,
                strDesc,
                nullptr,
                returnValue,
                std::vector<FuncParam>());
        return classTable->items[nameClass];
    };

    /* 1.0 Инициализация класса Int */
    addClass("JavaRTL/Int");
    /* 1.1 Конструктор */
    addMethod("JavaRTL/Int", "<init>", __CLASS, "(I)", "(I)V");
    /* 1.2 Арифметические операции */
    addMethod("JavaRTL/Int", "plus", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "minus", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "times", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "div", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Int;");
    /* 1.3 Унарные операции */
    addMethod("JavaRTL/Int", "uPlus", __CLASS, "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/Int", "uMinus", __CLASS, "()", "()LJavaRTL/Int;");
    /* 1.4 Массив */
    addMethod("JavaRTL/Int", "rangeTo", __ARRAY, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)[LJavaRTL/Int;");
    /* 1.5 Сравнение */
    addMethod("JavaRTL/Int", "greater", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "less", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "greaterEquals", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "lessEquals", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "equals", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Int", "notEquals", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Boolean;");

    /* 2.0 Инициализация класса Float */
    addClass("JavaRTL/Float");
    /* 2.1 Конструктор */
    addMethod("JavaRTL/Float", "<init>", __CLASS, "(F)", "(F)V");
    /* 2.2 Арифметические операции */
    addMethod("JavaRTL/Float", "plus", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "minus", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "times", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "div", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Int;)LJavaRTL/Float;");
    /* 2.3 Унарные операции */
    addMethod("JavaRTL/Float", "uPlus", __CLASS, "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/Float", "uMinus", __CLASS, "()", "()LJavaRTL/Float;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Float", "greater", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "less", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "greaterEquals", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "lessEquals", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "equals", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Float", "notEquals", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Boolean;");


    /* 3.0 Инициализация класса Double */
    addClass("JavaRTL/Double");
    /* 3.1 Конструктор */
    addMethod("JavaRTL/Double", "<init>", __CLASS, "(D)", "(D)V");
    /* 3.2 Арифметические операции */
    addMethod("JavaRTL/Double", "plus", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "minus", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "times", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "div", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Double;");
    /* 3.3 Унарные операции */
    addMethod("JavaRTL/Double", "uPlus", __CLASS, "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/Double", "uMinus", __CLASS, "()", "()LJavaRTL/Double;");
    /* 2.4 Сравнение */
    addMethod("JavaRTL/Double", "greater", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "less", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "greaterEquals",__CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "lessEquals", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "equals",__CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Double", "notEquals",__CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Boolean;");

    /* 4. Инициализация класса String */
    addClass("JavaRTL/String");

    addMethod("JavaRTL/String", "toInt", __CLASS, "()", "()LJavaRTL/Int;");
    addMethod("JavaRTL/String", "toFloat", __CLASS, "()", "()LJavaRTL/Float;");
    addMethod("JavaRTL/String", "toDouble", __CLASS, "()", "()LJavaRTL/Double;");
    addMethod("JavaRTL/String", "length", __CLASS, "()", "()I");

    /* 5. Инициализация класса Char */
    addClass("JavaRTL/Char");
    addMethod("JavaRTL/Char", "<init>", __CLASS, "(C)", "(C)V");

    /* 6. Инициализация класса Boolean */
    addClass("JavaRTL/Boolean");
    addMethod("JavaRTL/Boolean", "<init>", __CLASS, "(I)", "(I)V");  // true/false как 1/0
    addMethod("JavaRTL/Boolean", "not", __CLASS, "()", "()LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "and", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "or", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "equals", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");
    addMethod("JavaRTL/Boolean", "notEquals", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Boolean;");

    /* 7. Инициализация класса Any и Unit */
    addClass("JavaRTL/Any", true);
    addClass("JavaRTL/Unit");

    /* 8. I/O */
    addClass("JavaRTL/InputOutput");

    // print для всех типов
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "print", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // println для всех типов
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/Int;)", "(LJavaRTL/Int;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/Float;)", "(LJavaRTL/Float;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/Double;)", "(LJavaRTL/Double;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/String;)", "(LJavaRTL/String;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/Char;)", "(LJavaRTL/Char;)LJavaRTL/Unit;");
    addMethod("JavaRTL/InputOutput", "println", __CLASS, "(LJavaRTL/Boolean;)", "(LJavaRTL/Boolean;)LJavaRTL/Unit;");

    // readLine
    addMethod("JavaRTL/InputOutput", "readLine", __CLASS, "()", "()LJavaRTL/String;");

    return *classTable;
}