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

    auto addClass = [&](const std::string& name) {
        classTable->items[name] = new ClassTableElement();
        classTable->items[name]->clsName = name;
        classTable->items[name]->isOpen = 0;
        return classTable->items[name];
    };
    auto addMethod = [&](
        const std::string& nameClass,
        const std::string& nameMethod,
        TypeNodeType type,
        const std::string& desk,
        int methodName,
        int descriptorName,
        std::string strDesc,
        StmtListNode *start){
        classTable->items[nameClass]->methods->methods[nameMethod] =
            std::map<std::string, class MethodTableElement*>();
        returnValue = new TypeTable();
        returnValue->className = classTable->items[nameClass]->clsName;
        returnValue->type = type;
        classTable->items[nameClass]->methods->methods[nameMethod][desk] =
            new MethodTableElement(
                methodName,
                descriptorName,
                nameMethod,
                strDesc,
                start,
                returnValue,
                std::vector<FuncParam>());
        return classTable->items[nameClass];
    };

    /* 1.0 Инициализация класса Int */
    addClass("JavaRTL/Int");
    /* 1.1 Конструктор */
    addMethod("JavaRTL/Int", "<init>", TypeNodeType::__CLASS, "(I)", 0, 0, "(I)V", nullptr);
    /* 1.2 Сложение */
    addMethod("JavaRTL/Int", "plus", TypeNodeType::__CLASS, "(LJavaRTL/Int;)", 0, 0, "(LJavaRTL/Int;)LJavaRTL/Int;", nullptr);
    /* 1.3 Вычитание */
    /* 1.4 Умножение */
    /* 1.5 Деление */
    /* 1.6 Унарный плюс */
    /* 1.7 Унарный минус */
    /* 1.8 Массив */
    /* 1.9 Больше */
    /* 1.10 Меньше */
    /* 1.11 Больше равно */
    /* 1.12 Меньше равно */
    /* 1.13 Равно */
    /* 1.14 Не равно */

    /* 2.0 Инициализация класса Float */
    addClass("JavaRTL/Float");

    /* 3.0 Инициализация класса Double */
    addClass("JavaRTL/Double");


    return *classTable;
}
