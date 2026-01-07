#ifndef KOTLIN_COMPILER_LOCALVARIABLETABLEELEMENT_H
#define KOTLIN_COMPILER_LOCALVARIABLETABLEELEMENT_H
#include <string>
#include <../semantic/tables/type/SemanticType.h>

class LocalVariableTableElement {
public:
    std::string name;
    int id;
    SemanticType* type;
    int isConst;
    int isInitialized;

    LocalVariableTableElement(std::string name, int id, SemanticType * type, int isConst, int isInitialized);


};

#endif //KOTLIN_COMPILER_LOCALVARIABLETABLEELEMENT_H