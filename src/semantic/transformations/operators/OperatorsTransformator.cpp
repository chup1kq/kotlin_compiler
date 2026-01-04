#include "OperatorsTransformator.h"

#include <ios>

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
            expr->identifierName =  "lt";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _GREAT) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "gt";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _LESS_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "le";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _GREAT_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "ge";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "eq";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _NOT_EQUAL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "neq";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _DISJUNCTION) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "dis";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _CONJUNCTION) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "con";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }

        // ----------------------- присваивание ----------------------

        else if (expr->type == _PLUS_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* plusExpr = ExprNode::createExprNode(_PLUS, leftCopy, right);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = plusExpr;
        }
        else if (expr->type == _MINUS_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* minusExpr = ExprNode::createExprNode(_MINUS, leftCopy, right);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = minusExpr;
        }
        else if (expr->type == _MUL_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* mulExpr = ExprNode::createExprNode(_MUL, leftCopy, right);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = mulExpr;
        }
        else if (expr->type == _DIV_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* divExpr = ExprNode::createExprNode(_DIV, leftCopy, right);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = divExpr;
        }
        else if (expr->type == _MOD_ASSIGNMENT && expr->left) {
            ExprNode* left = expr->left;
            ExprNode* right = expr->right;
            ExprNode* leftCopy = left->clone();
            ExprNode* modExpr = ExprNode::createExprNode(_MOD, leftCopy, right);
            expr->type = _ASSIGNMENT;
            expr->left = left;
            expr->right = modExpr;
        }

        // ----------------------- Range для циклов ----------------------

        else if (expr->type == _RANGE) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "range";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _DOWN_TO) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "downTo";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _UNTIL) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "until";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
        else if (expr->type == _STEP) {
            expr->type = _FUNC_ACCESS;
            expr->identifierName =  "step";
            expr->params = new ExprListNode(expr->right);
            expr->right = nullptr;
        }
    }
}
