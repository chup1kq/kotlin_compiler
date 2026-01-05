#include "ASTTransformer.h"
#include "../../error/SemanticError.h"

void ASTTransformer::transform(KotlinFileNode* root) {
    if (!root || !root->topLevelList)
        throw SemanticError::emptyTree();

    transformFunctions(*root->topLevelList->functionList);
    transformClasses(*root->topLevelList->classList);
}


void ASTTransformer::transformClasses(std::list<ClassNode*> classes) {
    for (auto* cls : classes) {
        if (!cls) continue;

        if (cls->body) {
            if (cls->body->methods)
                transformFunctions(*cls->body->methods);
            if (cls->body->fields)
                transformStatements(*cls->body->fields->stmts);
            if (cls->body->constructors)
                transformConstructors(*cls->body->constructors);
            if (cls->isEnum && cls->body->enumEntries && cls->body->enumEntries->enumEntries)
                for (auto* e : *cls->body->enumEntries->enumEntries)
                    transformExpressions(e->params);
        }

        if (cls->primaryConstructor)
            transformConstructor(cls->primaryConstructor);
    }
}

void ASTTransformer::transformFunctions(std::list<FunNode*> functions) {
    for (auto* fun : functions) {
        if (!fun)
            continue;

        if (fun->body && fun->body->stmts)
            transformStatements(*fun->body->stmts);

        if (fun->args)
            transformVarDeclarations(*fun->args->decls);

    }

}


void ASTTransformer::transformConstructors(std::list<Constructor*> constructors) {
    for (auto* c : constructors)
        if (c) transformConstructor(c);
}

void ASTTransformer::transformConstructor(Constructor* c) {
    if (c->stmts)
        transformStatements(*c->stmts->stmts);

    if (c->args) {
        for (auto* arg : c->args->args) {
            if (auto* v = dynamic_cast<VarDeclaration*>(arg))
                transformVarDeclaration(v);
            else if (auto* s = dynamic_cast<StmtNode*>(arg))
                transformStatement(s);
        }
    }

    if (c->prevConstructorArgs)
        transformVarDeclarations(*c->prevConstructorArgs->decls);
}


void ASTTransformer::transformStatements(std::list<StmtNode*> stmts) {
    for (auto* s : stmts)
        if (s) transformStatement(s);
}

void ASTTransformer::transformStatement(StmtNode* stmt) {
    if (!stmt) return;

    switch (stmt->type) {

        case _IF_STMT:
            transformExpression(stmt->cond);
            if (stmt->trueStmtList)
                transformStatements(*stmt->trueStmtList->stmts);
            if (stmt->falseStmtList)
                transformStatements(*stmt->falseStmtList->stmts);
            break;

        case _EXPRESSION:
            transformExpression(stmt->expr);
            break;

        case _WHILE:
        case _DO_WHILE:
            transformExpression(stmt->cond);
            if (stmt->blockStmts)
                transformStatements(*stmt->blockStmts->stmts);
            break;

        case _FOR:
            transformVarDeclaration(stmt->forIterator);
            transformExpression(stmt->cond);
            if (stmt->forIteratorList)
                transformVarDeclarations(*stmt->forIteratorList->decls);
            if (stmt->blockStmts)
                transformStatements(*stmt->blockStmts->stmts);
            break;
        case _VAR:
        case _VAL:
            transformVarDeclaration(stmt->varDeclaration);
            break;

        case _RETURN:
            transformExpression(stmt->expr);
            break;

        default:
            break;
    }
}

void ASTTransformer::transformExpression(ExprNode* expr) {
    if (!expr) return;

    transformExpressionBody(expr);

    if (expr->left)
        transformExpression(expr->left);

    if (expr->right)
        transformExpression(expr->right);

    if (expr->params)
        transformExpressions(expr->params);

    if (expr->elements)
        transformExpressions(expr->elements);
}

void ASTTransformer::transformExpressions(ExprListNode* exprs) {
    if (!exprs || !exprs->exprs) return;

    for (auto* e : *exprs->exprs)
        transformExpression(e);
}

void ASTTransformer::transformVarDeclarations(std::list<VarDeclaration*> decls) {
    for (auto* d : decls)
        if (d) transformVarDeclaration(d);
}

void ASTTransformer::transformVarDeclaration(VarDeclaration* decl) {
    if (!decl) return;

    transformVarDeclarationBody(decl);

    if (decl->defaultValue)
        transformExpression(decl->defaultValue);
}
