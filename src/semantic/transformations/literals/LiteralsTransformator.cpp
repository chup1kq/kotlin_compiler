#include "LiteralsTransformator.h"

static void _replaceLiteralsToObjectsInKotlinElement(KotlinFileNode * root);

static void _replaceLiteralsToObjectsInKotlinElementList(KotlinElementList * root);

static void _replaceLiteralsToObjectsInExpression(ExprNode * root);

static void _replaceLiteralsToObjectsInExpressionList(ExprListNode * root);

static void _replaceLiteralsToObjectsInStatement(StmtNode * root);

static void _replaceLiteralsToObjectsInStatementList(StmtListNode * root);

static void _replaceLiteralsToObjectsInFunction(FunNode * root);

static void _replaceLiteralsToObjectsInClass(ClassNode * root);

static void _replaceLiteralsToObjectsInClassBody(ClassBodyNode * root);

static void _replaceLiteralsToObjectsInInheritance(Inheritance * root);

static void _replaceLiteralsToObjectsInConstructor(Constructor * root);

static void _replaceLiteralsToObjectsInConstructorArgs(ConstructorArgs * root);

static void _replaceLiteralsToObjectsInVarDeclarationList(VarDeclarationList * root);

static void _replaceLiteralsToObjectsInVarDeclaration(VarDeclaration * root);

void replaceLiteralsToObjects(KotlinFileNode * root) {
    _replaceLiteralsToObjectsInKotlinElement(root);
}

static void _replaceLiteralsToObjectsInKotlinElement(KotlinFileNode * root) {
    _replaceLiteralsToObjectsInKotlinElementList(root->topLevelList);
}

static void _replaceLiteralsToObjectsInKotlinElementList(KotlinElementList * root) {
    for (auto & cls : *root->classList) {
        _replaceLiteralsToObjectsInClass(cls);
    }
    for (auto & func : *root->functionList) {
        _replaceLiteralsToObjectsInFunction(func);
    }
}

static void _replaceLiteralsToObjectsInExpression(ExprNode * root) {
    if (root->type == _FUNC_CALL && root->params != nullptr) {
        _replaceLiteralsToObjectsInExpressionList(root->params);
    }

    if (root->left != nullptr) _replaceLiteralsToObjectsInExpression(root->left);
    if (root->right != nullptr) _replaceLiteralsToObjectsInExpression(root->right);

    if (root->type == _INTEGER_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Int";
        root->fromLiteral = _FROM_INT;
    } else if (root->type == _DOUBLE_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Double";
        root->fromLiteral = _FROM_DOUBLE;
    } else if (root->type == _BOOL_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Boolean";
        root->fromLiteral = _FROM_BOOLEAN;
    } else if (root->type == _CHAR_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Char";
        root->fromLiteral = _FROM_CHAR;
    } else if (root->type == _STRING_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "String";
        root->fromLiteral = _FROM_STRING;
    }

    if (root->elements != nullptr) _replaceLiteralsToObjectsInExpressionList(root->elements);
}

static void _replaceLiteralsToObjectsInExpressionList(ExprListNode * root) {
    for (auto& expr : *root->exprs ) {
        _replaceLiteralsToObjectsInExpression(expr);
    }
}

static void _replaceLiteralsToObjectsInStatement(StmtNode * root) {
    switch (root->type) {
        case _IF_STMT:
            _replaceLiteralsToObjectsInExpression(root->cond);
            _replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _EXPRESSION:
            _replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _WHILE:
            _replaceLiteralsToObjectsInExpression(root->cond);
            _replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _FOR:
            _replaceLiteralsToObjectsInExpression(root->cond);
            _replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _DO_WHILE:
            _replaceLiteralsToObjectsInExpression(root->cond);
            _replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _VAR:
            _replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _VAL:
            _replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _RETURN:
            _replaceLiteralsToObjectsInExpression(root->expr);
            break;
        default: break;
    }
}

static void _replaceLiteralsToObjectsInStatementList(StmtListNode * root) {
    for (auto& stmt : *root->stmts ) {
        _replaceLiteralsToObjectsInStatement(stmt);
    }
}

static void _replaceLiteralsToObjectsInFunction(FunNode * root) {
    if (root != nullptr) _replaceLiteralsToObjectsInStatementList(root->body);
}

static void _replaceLiteralsToObjectsInClass(ClassNode * root) {
    /*TODO modifiers*/
    _replaceLiteralsToObjectsInInheritance(root->inheritance);
    _replaceLiteralsToObjectsInClassBody(root->body);
    _replaceLiteralsToObjectsInConstructor(root->primaryConstructor);
}

static void _replaceLiteralsToObjectsInInheritance(Inheritance * root) {
    _replaceLiteralsToObjectsInExpressionList(root->constructorParams);
}

static void _replaceLiteralsToObjectsInClassBody(ClassBodyNode * root) {
    for (auto & constr : *root->constructors) {
        _replaceLiteralsToObjectsInConstructor(constr);
    }
    _replaceLiteralsToObjectsInStatementList(root->fields);
    for (auto & method : *root->methods) {
        _replaceLiteralsToObjectsInFunction(method);
    }
}

static void _replaceLiteralsToObjectsInConstructor(Constructor * root) {
    /*TODO modifiers prevConstructorType prevConstructorArgs*/
    _replaceLiteralsToObjectsInConstructorArgs(root->args);
    _replaceLiteralsToObjectsInStatementList(root->stmts);
}

static void _replaceLiteralsToObjectsInConstructorArgs(ConstructorArgs * root) {
    for (auto & arg : root->args) {
        if (VarDeclaration * varDecl = dynamic_cast<VarDeclaration*>(arg)) {
            _replaceLiteralsToObjectsInVarDeclaration(varDecl);
        } else if (StmtNode * stmt = dynamic_cast<StmtNode*>(arg)) {
            _replaceLiteralsToObjectsInStatement(stmt);
        }
    }
}

static void _replaceLiteralsToObjectsInVarDeclarationList(VarDeclarationList * root) {
    for (auto & decl : *root->decls) {
        _replaceLiteralsToObjectsInVarDeclaration(decl);
    }
}

static void _replaceLiteralsToObjectsInVarDeclaration(VarDeclaration * root) {
    /*TODO не знаю*/
}