#include "MethodTable.h"

MethodTable::MethodTable() {
    methods = std::map<
        std::string,
        std::map<std::string, MethodTableElement*>
    >();
}

void MethodTable::addMethod(
    const std::string& methodName,
    const std::string& descriptor,
    MethodTableElement* element
) {
    // если метода с таким именем ещё нет — создаём
    if (!methods.count(methodName)) {
        methods[methodName] = {};
    }

    // кладём конкретную перегрузку
    methods[methodName][descriptor] = element;
}
