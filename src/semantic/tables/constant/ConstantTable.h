#ifndef KOTLIN_COMPILER_CONSTANTTABLE_H
#define KOTLIN_COMPILER_CONSTANTTABLE_H
#include <map>
#include <string>

#include "ConstantTableElement.h"

class ConstantTable {
public:
    int maxId = 1;

    std::map<int, ConstantTableElement*> items;

    ConstantTable();

    // return - номер в таблице констант, найденный или добавленный
    int findOrAddConstant(
        ConstantType type,
        std::string utf8String = "",
        int intValue = 0,
        double doubleValue = 0.0,
        int firstRef = 0,
        int secondRef = 0
    );

private:
    // return - найденный номер в таблице констант
    int findConstant(
        ConstantType type,
        std::string utf8String = "",
        int intValue = 0,
        double doubleValue = 0.0,
        int firstRef = 0,
        int secondRef = 0
    );
};

#endif //KOTLIN_COMPILER_CONSTANTTABLE_H