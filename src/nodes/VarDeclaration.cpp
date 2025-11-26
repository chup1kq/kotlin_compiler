#include "VarDeclaration.h"

VarDeclaration *VarDeclaration::createVarDeclaration(std::string varId, TypeNode varType, ExprNode *defaultValue) {
    VarDeclaration *varDeclaration = new VarDeclaration();
    varDeclaration->varId = varId;
    varDeclaration->varType = varType;
    varDeclaration->defaultValue = defaultValue;
    return varDeclaration;
}
