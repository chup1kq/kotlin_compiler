#ifndef KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H
#define KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H
#include <string>

enum ConstantType {
    UTF8 = 1,
    Class = 7,
    MethodRef = 10,
    FieldRef = 9,
    NameAndType = 12,
    Integer = 3,
    Float = 4,
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
        std::string utf8String = "",
        int intValue = 0,
        double doubleValue = 0.0,
        int firstRef = 0,
        int secondRef = 0
    );
};


#endif //KOTLIN_COMPILER_CONSTANTTABLEELEMENT_H