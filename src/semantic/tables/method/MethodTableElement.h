#ifndef KOTLIN_COMPILER_METHODTABLEELEMENT_H
#define KOTLIN_COMPILER_METHODTABLEELEMENT_H
#include <vector>

#include "StmtListNode.h"
#include "../params/FuncParam.h"
#include "../type/SemanticType.h"
#include "../variables/LocalVariableTable.h"

class MethodTableElement {
public:
    int methodName;
    int descriptor;
    std::string strName;
    std::string strDesc;
    StmtListNode * start;
    SemanticType * retType = nullptr;
    vector<FuncParam*> params;
    LocalVariableTable * localVarTable = nullptr;
    int isFirst;

    MethodTableElement(int nameConst,
        int descConst,
        std::string name,
        std::string strName);

    MethodTableElement(int methodName,
        int descriptorName,
        std::string strName,
        std::string strDesc,
        StmtListNode* start,
        SemanticType* retType,
        vector<FuncParam*> params);
};

#endif //KOTLIN_COMPILER_METHODTABLEELEMENT_H