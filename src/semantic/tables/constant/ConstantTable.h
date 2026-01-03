#ifndef KOTLIN_COMPILER_CONSTANTTABLE_H
#define KOTLIN_COMPILER_CONSTANTTABLE_H
#include <map>

#include "ConstantTableElement.h"

class ConstantTable {
public:
    int maxId = 1;

    static std::map<int, ConstantTableElement*> items;

    ConstantTable();

    // return - номер в таблице констант, найденный или добавленный
    int findOrAddConstant(
        ConstantType type,
        std::string utf8String = nullptr,
        int intValue = NULL,
        double doubleValue = NULL,
        int firstRef = NULL,
        int secondRef = NULL
    );

private:
    // return - найденный номер в таблице констант
    int findConstant(
        ConstantType type,
        std::string utf8String = nullptr,
        int intValue = NULL,
        double doubleValue = NULL,
        int firstRef = NULL,
        int secondRef = NULL
    );
};


#endif //KOTLIN_COMPILER_CONSTANTTABLE_H