#include "OperatorsTransformator.h"

#include "../../error/SemanticError.h"

void OperatorsTransformator::replaceOperators(KotlinFileNode *root) {
    if (root == nullptr ||
        root->topLevelList == nullptr ||
        (root->topLevelList->classList->empty() &&
        root->topLevelList->functionList->empty())
    ) {
        throw SemanticError::emptyTree();
    }

    replaceOperatorsInFunctions(*root->topLevelList->functionList);
    replaceOperatorsInClasses(*root->topLevelList->classList);
}

void OperatorsTransformator::replaceOperatorsInClasses(std::list<ClassNode*> classes) {
    for (auto& cls : classes) {
        replaceOperatorsInFunctions(*cls->body->methods);
    }
}

void OperatorsTransformator::replaceOperatorsInFunctions(std::list<FunNode*> functions) {
    for (auto& function : functions ) {
        replaceOperatorsInStatements(*function->body->stmts);
    }
}

void OperatorsTransformator::replaceOperatorsInStatements(std::list<StmtNode*> stmts) {
    for (auto& stmt : stmts) {
        replaceOperatorsInStatement(stmt);
    }
}

void OperatorsTransformator::replaceOperatorsInStatement(StmtNode *stmt) {
    if (!stmt) return;

    switch (stmt->type) {

        case _IF_STMT:
            if (stmt->cond)
                replaceOperatorsInExpression(stmt->cond);

            if (stmt->trueStmtList && stmt->trueStmtList->stmts)
                replaceOperatorsInStatements(*stmt->trueStmtList->stmts);

            if (stmt->falseStmtList && stmt->falseStmtList->stmts)
                replaceOperatorsInStatements(*stmt->falseStmtList->stmts);
            break;

        case _EXPRESSION:
            if (stmt->expr)
                replaceOperatorsInExpression(stmt->expr);
            break;

        case _WHILE:
        case _DO_WHILE:
            if (stmt->cond)
                replaceOperatorsInExpression(stmt->cond);

            if (stmt->cycleSingleStmt)
                replaceOperatorsInStatement(stmt->cycleSingleStmt);

            if (stmt->blockStmts && stmt->blockStmts->stmts)
                replaceOperatorsInStatements(*stmt->blockStmts->stmts);
            break;

        case _FOR:
            if (stmt->forIterator && stmt->forIterator->defaultValue)
                replaceOperatorsInExpression(stmt->forIterator->defaultValue);

            if (stmt->forIteratorList && stmt->forIteratorList->decls)
                replaceOperatorsInVarDeclarations(*stmt->forIteratorList->decls);

            if (stmt->cond)
                replaceOperatorsInExpression(stmt->cond);

            if (stmt->cycleSingleStmt)
                replaceOperatorsInStatement(stmt->cycleSingleStmt);

            if (stmt->blockStmts && stmt->blockStmts->stmts)
                replaceOperatorsInStatements(*stmt->blockStmts->stmts);
            break;

        case _VAR:
        case _VAL:
            if (stmt->varDeclaration && stmt->varDeclaration->defaultValue)
                replaceOperatorsInExpression(stmt->varDeclaration->defaultValue);
            break;

        case _RETURN:
            if (stmt->expr)
                replaceOperatorsInExpression(stmt->expr);
            break;

        case _EMPTY:
        case _BREAK:
        case _CONTINUE:
            break;
    }
}

void OperatorsTransformator::replaceOperatorsInExpression(ExprNode *expr) {
    if (!expr) return;

    if (expr->right) {
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

    }

    if (expr->left)
        replaceOperatorsInExpression(expr->left);

    if (expr->right)
        replaceOperatorsInExpression(expr->right);

    if (expr->params && expr->params->exprs)
        replaceOperatorsInExpressions(expr->params);
}


void OperatorsTransformator::replaceOperatorsInVarDeclarations(std::list<VarDeclaration*> decls) {
    for (auto& decl : decls) {
        replaceOperatorsInExpression(decl->defaultValue);
    }
}

void OperatorsTransformator::replaceOperatorsInExpressions(ExprListNode *exprs) {
    for (auto& expr : *exprs->exprs) {
        replaceOperatorsInExpression(expr);
    }
}
