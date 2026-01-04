#include "LiteralsTransformator.h"
#include "../../error/SemanticError.h"

void LiteralsTransformator::transformVarDeclarationBody(VarDeclaration *decl) {}

void LiteralsTransformator::transformExpressionBody(ExprNode* expr) {
    if (expr->left != nullptr) transformExpression(expr->left);
    if (expr->right != nullptr) transformExpression(expr->right);

    if (expr->type == _INTEGER_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Int";
        expr->fromLiteral = _FROM_INT;
    } else if (expr->type == _FLOAT_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Float";
        expr->fromLiteral = _FROM_FLOAT;
    } else if (expr->type == _DOUBLE_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Double";
        expr->fromLiteral = _FROM_DOUBLE;
    } else if (expr->type == _BOOL_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Boolean";
        expr->fromLiteral = _FROM_BOOLEAN;
    } else if (expr->type == _CHAR_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Char";
        expr->fromLiteral = _FROM_CHAR;
    } else if (expr->type == _STRING_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "String";
        expr->fromLiteral = _FROM_STRING;
    }
}