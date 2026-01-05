#include "LiteralsTransformator.h"
#include "../../error/SemanticError.h"

void LiteralsTransformator::transformVarDeclarationBody(VarDeclaration *decl) {}

void LiteralsTransformator::transformExpressionBody(ExprNode* expr) {
    if (expr->type == _INTEGER_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Int";
        expr->fromLiteral = _FROM_INT;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createIntNode(expr->intValue));
    } else if (expr->type == _FLOAT_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Float";
        expr->fromLiteral = _FROM_FLOAT;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createFloatNode(expr->floatValue));
    } else if (expr->type == _DOUBLE_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Double";
        expr->fromLiteral = _FROM_DOUBLE;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createDoubleNode(expr->doubleValue));
    } else if (expr->type == _BOOL_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Boolean";
        expr->fromLiteral = _FROM_BOOLEAN;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createBoolNode(expr->boolValue));
    } else if (expr->type == _CHAR_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "Char";
        expr->fromLiteral = _FROM_CHAR;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createCharNode(expr->charValue));
    } else if (expr->type == _STRING_LITERAL) {
        expr->type = _FUNC_CALL;
        expr->identifierName = "String";
        expr->fromLiteral = _FROM_STRING;
        expr->params = ExprListNode::addExprToList(nullptr, ExprNode::createStringNode(expr->stringValue));
    }
}