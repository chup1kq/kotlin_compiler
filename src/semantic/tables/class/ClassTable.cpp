#include "ClassTable.h"

#include "ClassTableElement.h"
#include "../constant/ConstantTable.h"

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
