#ifndef KOTLIN_COMPILER_METHODTABLEELEMENT_H
#define KOTLIN_COMPILER_METHODTABLEELEMENT_H
#include <vector>

#include "StmtListNode.h"
#include "../variables/FuncParam.h"
#include "../variables/TypeTable.h"
#include "../variables/LocalVariableTable.h"

class MethodTableElement {
public:
    int methodName = NULL;
    int descriptor = NULL;
    std::string strName;
    std::string strDesc;
    StmtListNode * start;
    TypeTable * retType = nullptr;
    vector<FuncParam> params;
    LocalVariableTable * localVarTable = nullptr;
    int isFirst = NULL;

    MethodTableElement(int nameConst,
        int descConst,
        std::string name,
        std::string strName);
};

#endif //KOTLIN_COMPILER_METHODTABLEELEMENT_H