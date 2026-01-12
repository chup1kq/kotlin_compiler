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
    std::string fullKey = methodName + "_" + descriptor;
    return methods.count(fullKey) > 0;
}

MethodTableElement* MethodTable::getMethod(const std::string& methodName, const std::string& descriptor) const {
    std::string fullKey = methodName + "_" + descriptor;
    auto it = methods.find(fullKey);
    return (it != methods.end()) ? it->second : nullptr;
}
