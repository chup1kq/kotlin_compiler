#ifndef KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H
#define KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H
#include <string>

enum ConstantType {
    UTF8 = 1,
    Class = 2,
    MethodRef = 3,
    FieldRef = 4,
    NameAndType = 5,
    Integer = 6,
    Double = 7,
    String = 8,
};

class ConstantTableElement {
public:
    int id; // номер в таблице констант
    ConstantType type;
    std::string utf8String; // для UTF-8 констант
    int intValue;
    double doubleValue;
    int firstRef;
    int secondRef;

    ConstantTableElement(
        int id,
        ConstantType type,
        std::string utf8String = nullptr,
        int intValue = NULL,
        double doubleValue = NULL,
        int firstRef = NULL,
        int secondRef = NULL
    );
};


#endif //KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H