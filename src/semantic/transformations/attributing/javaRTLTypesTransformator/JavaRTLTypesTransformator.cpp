#include "JavaRTLTypesTransformator.h"

#include <iostream>

#include "../../../error/SemanticError.h"

void JavaRTLTypesTransformator::transformVarDeclarationBody(VarDeclaration *decl) {
    if (decl->varType)
        replaceToRTLInTypeNode(decl->varType);
}

void JavaRTLTypesTransformator::transformExpressionBody(ExprNode* expr) {}

void JavaRTLTypesTransformator::replaceToRTLInTypeNode(TypeNode *typeNode) {
    if (typeNode->type == _ARRAY) {
        std::cout << "In Array" << std::endl;
        replaceToRTLInTypeNode(typeNode->arrayType);
        return;
    }

    switch (typeNode->type) {
        case _INT:
            typeNode->customName = "JavaRTL/Int";
            std::cout << "In Int" << std::endl;
            break;
        case _FLOAT:
            typeNode->customName = "JavaRTL/Float";
            break;
        case _DOUBLE:
            typeNode->customName = "JavaRTL/Double";
            break;
        case _CHAR:
            typeNode->customName = "JavaRTL/Char";
            break;
        case _STRING:
            typeNode->customName = "JavaRTL/String";
            break;
        case _BOOLEAN:
            typeNode->customName = "JavaRTL/Boolean";
            break;
        case _UNIT:
            typeNode->customName = "JavaRTL/Unit";
            break;

        default:
            break;
    }
}
