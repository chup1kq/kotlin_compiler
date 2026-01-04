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
        // TODO добавить обработку полей и конструкторов класса
        if (!cls || !cls->body || !cls->body->methods)
            continue;

        replaceOperatorsInFunctions(*cls->body->methods);
    }
}


void OperatorsTransformator::replaceOperatorsInFunctions(std::list<FunNode*> functions) {
    for (auto& function : functions ) {
        if (!function || !function->body || !function->body->stmts)
            continue;

        replaceOperatorsInStatements(*function->body->stmts);
    }
}


void OperatorsTransformator::replaceOperatorsInStatements(std::list<StmtNode*> stmts) {
    for (auto& stmt : stmts) {
        if (stmt)
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

    if (expr->left)
        replaceOperatorsInExpression(expr->left);

    if (expr->right)
        replaceOperatorsInExpression(expr->right);

    if (expr->params && expr->params->exprs)
        replaceOperatorsInExpressions(expr->params);
}


void OperatorsTransformator::replaceOperatorsInVarDeclarations(std::list<VarDeclaration*> decls) {
    for (auto& decl : decls) {
        if (decl && decl->defaultValue)
            replaceOperatorsInExpression(decl->defaultValue);
    }
}


void OperatorsTransformator::replaceOperatorsInExpressions(ExprListNode *exprs) {
    if (!exprs || !exprs->exprs)
        return;

    for (auto& expr : *exprs->exprs) {
        if (expr)
            replaceOperatorsInExpression(expr);
    }
}

