#include "MethodTable.h"

MethodTable::MethodTable() {
    methods = std::map<std::string,
              std::map<std::string, MethodTableElement*>>();
}
