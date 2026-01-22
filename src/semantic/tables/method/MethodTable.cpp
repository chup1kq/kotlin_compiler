#include "MethodTable.h"

#include "../../error/SemanticError.h"

MethodTable::MethodTable() {
    // ✅ УПРОЩЕНО: обычная map без вложенности
    methods = std::map<std::string, MethodTableElement*>();
}

void MethodTable::addMethod(
    const std::string& methodName,
    const std::string& descriptor,
    MethodTableElement* element
) {
    // ✅ КЛЮЧ = methodName + "_" + descriptor (убрана вложенность)
    std::string fullKey = methodName + "_" + descriptor;

    // Проверяем уникальность (было бы перегрузка)
    if (methods.count(fullKey)) {
        throw SemanticError::methodAlreadyExists(fullKey);
    }

    methods[fullKey] = element;
}

bool MethodTable::contains(const std::string& methodName, const std::string& descriptor) const {
    // std::string fullKey = methodName + "_" + descriptor;
    // return methods.count(fullKey) > 0;
    for (auto& m : methods) {
        if (m.second->strName == methodName && m.second->getParamsFromDescriptor() == descriptor) {
            return true;
        }
        if (m.second->strName == MethodTableElement::transformNameToConstructorIfNeeded(methodName) &&
            m.second->getParamsFromDescriptor() == MethodTableElement::transformRTLTypeToJVMType(descriptor)
        ) {
            return true;
        }
    }

    return false;
}

bool MethodTable::contains(const std::string &methodName) const {
    for (auto& m : methods) {
        if (m.second->strName == methodName) {
            return true;
        }
        if (m.second->strName == MethodTableElement::transformNameToConstructorIfNeeded(methodName)) {
            return true;
        }
    }

    return false;
}


MethodTableElement* MethodTable::getMethod(const std::string& methodName, const std::string& paramsPart) const {
    // Формируем префикс поиска: "methodName_(params)"
    std::string prefix = methodName + "_" + paramsPart;

    // lower_bound находит первый ключ >= prefix
    auto it = methods.lower_bound(prefix);

    // Проверяем, начинается ли ключ с префикса
    while (it != methods.end()) {
        const std::string& key = it->first;
        if (key.size() < prefix.size() || key.compare(0, prefix.size(), prefix) != 0) {
            break;  // Ключи больше префикса или не совпадают
        }
        return it->second;  // Найден подходящий метод (первый по алфавиту)
    }

    return nullptr;  // Не найден
}

MethodTableElement* MethodTable::getMethod(const std::string& methodName) const {
    auto it = methods.lower_bound(methodName);

    // Проверяем, начинается ли ключ с префикса
    while (it != methods.end()) {
        const std::string& key = it->first;
        if (key.size() < methodName.size() || key.compare(0, methodName.size(), methodName) != 0) {
            break;  // Ключи больше префикса или не совпадают
        }
        return it->second;  // Найден подходящий метод (первый по алфавиту)
    }

    return nullptr;  // Не найден
}
