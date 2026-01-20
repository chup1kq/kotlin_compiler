#include "OperatorsTransformator.h"

#include <ios>
#include <iostream>
#include <ostream>

#include "../../error/SemanticError.h"

void OperatorsTransformator::transformVarDeclarationBody(VarDeclaration *decl) {}

void OperatorsTransformator::transformExpressionBody(ExprNode* expr) {

    // ----------------------- унарные операции -----------------------

    if (expr->left) {
        if (expr->type == _UNARY_PLUS) {
            ExprNode* operand = expr->left;
            expr->type = _FUNC_ACCESS;
            expr->identifierName = "uPlus";
            expr->left = operand;
            expr->params = new ExprListNode();
            expr->right = nullptr;
        }
        else if (expr->type == _UNARY_MINUS) {
            ExprNode* operand = expr->left;
            expr->type = _FUNC_ACCESS;
            expr->identifierName = "uMinus";
            expr->left = operand;
            expr->params = new ExprListNode();
            expr->right = nullptr;
        }
        else if (expr->type == _NOT) {
            ExprNode* operand = expr->left;
            expr->type = _FUNC_ACCESS;
            expr->identifierName = "not";
            expr->left = operand;
            expr->params = new ExprListNode();
            expr->right = nullptr;
        }
    }

    // ----------------------- бинарные операции -----------------------

    if (expr->right) {

        // ----------------------- математические операции -----------------------

        if (expr->type == _PLUS) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "plus";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _MINUS) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "minus";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _MUL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "mul";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _DIV) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "div";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _MOD) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "mod";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }

        // ----------------------- логические операции -----------------------

        else if (expr->type == _LESS) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "less";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _GREAT) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "greater";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _LESS_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "lessEquals";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _GREAT_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "greaterEquals";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "equals";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _NOT_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "notEquals";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _DISJUNCTION) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "or";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _CONJUNCTION) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "and";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }

        // ----------------------- присваивание ----------------------

        else if (expr->type == _PLUS_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* plusExpr = ExprNode::createMethodAccessExprNode("plus", new ExprListNode(right), leftCopy);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = plusExpr;
        }
        else if (expr->type == _MINUS_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* minusExpr = ExprNode::createMethodAccessExprNode("minus", new ExprListNode(right), leftCopy);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = minusExpr;
        }
        else if (expr->type == _MUL_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* mulExpr = ExprNode::createMethodAccessExprNode("mul", new ExprListNode(right), leftCopy);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = mulExpr;
        }
        else if (expr->type == _DIV_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* divExpr = ExprNode::createMethodAccessExprNode("div", new ExprListNode(right), leftCopy);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = divExpr;
        }
        else if (expr->type == _MOD_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* modExpr = ExprNode::createMethodAccessExprNode("mod", new ExprListNode(right), leftCopy);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = modExpr;
        }

        // ----------------------- Range для циклов ----------------------

        // TODO зарефакторить, чтобы перезаписывался expr-> либо внутри вспомогательного метода, либо снаружи
        else if (expr->type == _RANGE) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "range";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
            // ExprNode* newExpr = transformRangeToArrayOf(expr);
            //
            // expr->type = newExpr->type;
            // expr->typeElements = newExpr->typeElements;
            // expr->elements = newExpr->elements;
            //
            // expr->left = nullptr;
            // expr->right = nullptr;
            //
            // delete newExpr;
        }
        else if (expr->type == _DOWN_TO) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "downTo";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;

            // ExprNode* newExpr = transformRangeToArrayOf(expr);
            //
            // expr->type = newExpr->type;
            // expr->typeElements = newExpr->typeElements;
            // expr->elements = newExpr->elements;
            //
            // expr->left = nullptr;
            // expr->right = nullptr;
            //
            // delete newExpr;
        }
        else if (expr->type == _UNTIL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "until";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;

            // ExprNode* newExpr = transformRangeToArrayOf(expr);
            //
            // expr->type = newExpr->type;
            // expr->typeElements = newExpr->typeElements;
            // expr->elements = newExpr->elements;
            //
            // expr->left = nullptr;
            // expr->right = nullptr;
            //
            // delete newExpr;
        }
        else if (expr->type == _STEP) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "step";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;

            // addStepToArrayOf(expr);
        }
    }
}

ExprNode *OperatorsTransformator::transformRangeToArrayOf(ExprNode *range) {
    if (range->type == _RANGE) {
        return createArrayOfFromRange(range->left, range->right);
    }

    if (range->type == _DOWN_TO) {
        return createArrayOfFromRange(range->right, range->left);
    }

    if (range->type == _UNTIL) {
        range->right->intValue--;
        return createArrayOfFromRange(range->left, range->right);
    }
}

ExprNode *OperatorsTransformator::createArrayOfFromRange(ExprNode *start, ExprNode *end) {
    int startNumber = start->intValue;
    int endNumber = end->intValue;

    ExprListNode* exprs = new ExprListNode();
    for (int i = startNumber; i <= endNumber; i++) {
        std::cout << i << std::endl;
        ExprListNode::addExprToList(exprs, ExprNode::createIntNode(i));
    }

    return ExprNode::createArrayExprNode(
        TypeNode::createType(_INT, false, ""),
        exprs
    );
}

void OperatorsTransformator::addStepToArrayOf(ExprNode *expr) {
    if (expr->type != _STEP) {
        return;
    }

    int step = expr->right->intValue;
    if (step < 1) {
        throw SemanticError::invalidStepValue(step);
    }

    ExprListNode* elementsList = expr->left->elements;
    if (!elementsList || !elementsList->exprs) {
        return;
    }

    std::list<ExprNode*>& exprs = *elementsList->exprs;

    // Создаем новый список только с нужными элементами
    ExprListNode* newExprs = new ExprListNode();
    int index = 0;

    for (auto* element : exprs) {
        if (index % step == 0) {  // Берем каждый step-й элемент (0, step, 2*step...)
            ExprListNode::addExprToList(newExprs, element);
        }
        index++;
    }

    expr->type = _ARRAY_EXPR;
    expr->elements = newExprs;
    expr->typeElements = TypeNode::createType(_INT, false, "");
    expr->left = nullptr;
    expr->right = nullptr;
    expr->identifierName = "";
}
