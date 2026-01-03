#include "../ClassTable.h"

SemanticError* ClassTable::buildClassTable(KotlinFileNode* root) {
    // добавляем все классы
    for (ClassNode* cls : *root->topLevelList->classList) {
        const std::string& className = cls->name;

        // проверка нет ли уже такого класса
        if (exists(className)) {
            return SemanticError::redefinition(className);
        }

        ClassTableElement* elem = new ClassTableElement(
            className,
            cls
        );

        // добавляем в таблицу классов
        add(className, elem);
    }

    // вот тут (наверное) нужно добавить все методы в Unit класс
}

bool ClassTable::exists(const std::string& name) {
    return items.find(name) != items.end();
}


void ClassTable::add(const std::string& name, ClassTableElement* elem) {
    items[name] = elem;
}