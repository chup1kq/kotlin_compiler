#include "BaseTypesAttributing.h"

void BaseTypesAttributing::transformExpressionBody(ExprNode *expr) {
    auto attribute = [&](const std::string& name) {
        expr->typeElements = new TypeNode();
        expr->typeElements->isArray = false;
        expr->typeElements->arrayType = nullptr;
        expr->typeElements->customName = name;
        return expr;
    };

    if (expr->fromLiteral == _FROM_INT) attribute("JavaRTL/Int");
    else if (expr->fromLiteral == _FROM_FLOAT) attribute("JavaRTL/Float");
    else if (expr->fromLiteral == _FROM_DOUBLE) attribute("JavaRTL/Double");
    else if (expr->fromLiteral == _FROM_BOOLEAN) attribute("JavaRTL/Boolean");
    else if (expr->fromLiteral == _FROM_CHAR) attribute("JavaRTL/Char");
    else if (expr->fromLiteral == _FROM_STRING) attribute("JavaRTL/String");
    else if (expr->fromLiteral == _FROM_UNIT) attribute("JavaRTL/Unit");

    if (expr->left != nullptr) transformExpressionBody(expr->left);
    if (expr->right != nullptr) transformExpressionBody(expr->right);
    if (expr->params != nullptr) {
        for (auto param : *expr->params->exprs) {
            if (param == nullptr) continue;

            transformExpressionBody(param);
        }
    };
}

void BaseTypesAttributing::transformVarDeclarationBody(VarDeclaration *decl) {}
