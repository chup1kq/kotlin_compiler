#include "VarDeclaration.h"

VarDeclaration *VarDeclaration::createVarDeclaration(std::string varId, TypeNode* varType, ExprNode *defaultValue) {
    VarDeclaration *varDeclaration = new VarDeclaration();
    varDeclaration->varId = varId;
    varDeclaration->varType = varType;
    varDeclaration->defaultValue = defaultValue;
    return varDeclaration;
}

string VarDeclaration::getDotLabel() const {
    return varId;
}

string VarDeclaration::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, varType, "type");
    addDotChild(dot, defaultValue, "defaultValue");

    return dot;
}
