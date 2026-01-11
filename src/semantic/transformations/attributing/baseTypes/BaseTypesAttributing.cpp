#include "BaseTypesAttributing.h"

void BaseTypesAttributing::transformExpressionBody(ExprNode *expr) {
    // Пропускаем, если тип уже установлен
    if (expr->typeElements && expr->typeElements->customName != "") {
        return;
    }

    // === МАССИВЫ: выводим тип по элементам ===
    if (expr->type == _ARRAY_EXPR && expr->elements && expr->elements->exprs && !expr->elements->exprs->empty()) {
        ExprNode* firstElement = expr->elements->exprs->front();

        // Рекурсивно атрибутируем первый элемент
        transformExpressionBody(firstElement);

        // Копируем тип элемента для массива
        if (firstElement->typeElements) {
            expr->typeElements = firstElement->typeElements->clone();
            return;
        }
    }

    // === Литералы ===
    auto attribute = [&](const std::string& name) {
        expr->typeElements = new TypeNode();
        expr->typeElements->isArray = false;
        expr->typeElements->arrayType = nullptr;
        expr->typeElements->customName = name;
    };

    if (expr->fromLiteral == _FROM_INT) attribute("JavaRTL/Int");
    else if (expr->fromLiteral == _FROM_FLOAT) attribute("JavaRTL/Float");
    else if (expr->fromLiteral == _FROM_DOUBLE) attribute("JavaRTL/Double");
    else if (expr->fromLiteral == _FROM_BOOLEAN) attribute("JavaRTL/Boolean");
    else if (expr->fromLiteral == _FROM_CHAR) attribute("JavaRTL/Char");
    else if (expr->fromLiteral == _FROM_STRING) attribute("JavaRTL/String");
    else if (expr->fromLiteral == _FROM_UNIT) attribute("JavaRTL/Unit");
}

void BaseTypesAttributing::transformVarDeclarationBody(VarDeclaration *decl) {}
