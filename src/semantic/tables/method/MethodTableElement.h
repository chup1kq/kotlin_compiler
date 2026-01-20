#ifndef KOTLIN_COMPILER_METHODTABLEELEMENT_H
#define KOTLIN_COMPILER_METHODTABLEELEMENT_H
#include <vector>

#include "ModifierMap.h"
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
    LocalVariableTable * localVarTable = new LocalVariableTable();
    int isFirst;
    int superConstructorCall = 0;
    ModifierMap* modifierMap;

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
        vector<FuncParam*> params,
        ModifierMap* modifierMap);

    std::string getParamsFromDescriptor();
    static std::string transformNameToConstructorIfNeeded(const std::string& methodName);
    static std::string transformRTLTypeToJVMType(const std::string& descriptor);
};

#endif //KOTLIN_COMPILER_METHODTABLEELEMENT_H