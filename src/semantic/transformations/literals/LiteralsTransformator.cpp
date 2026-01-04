#include "LiteralsTransformator.h"

void LiteralsTransformator::replaceLiteralsToObjects(KotlinFileNode * root) {
    replaceLiteralsToObjectsInKotlinElement(root);
}

void LiteralsTransformator::replaceLiteralsToObjectsInKotlinElement(KotlinFileNode * root) {
    replaceLiteralsToObjectsInKotlinElementList(root->topLevelList);
}

void LiteralsTransformator::replaceLiteralsToObjectsInKotlinElementList(KotlinElementList * root) {
    for (auto & cls : *root->classList) {
        replaceLiteralsToObjectsInClass(cls);
    }
    for (auto & func : *root->functionList) {
        replaceLiteralsToObjectsInFunction(func);
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInExpression(ExprNode * root) {
    if (root->type == _FUNC_CALL && root->params != nullptr) {
        replaceLiteralsToObjectsInExpressionList(root->params);
    }

    if (root->left != nullptr) replaceLiteralsToObjectsInExpression(root->left);
    if (root->right != nullptr) replaceLiteralsToObjectsInExpression(root->right);

    if (root->type == _INTEGER_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Int";
        root->fromLiteral = _FROM_INT;
    } else if (root->type == _FLOAT_LITERAL) {
        root->type = _FUNC_CALL;
        root->identifierName = "Float";
        root->fromLiteral = _FROM_FLOAT;
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

    if (root->elements != nullptr) replaceLiteralsToObjectsInExpressionList(root->elements);
}

void LiteralsTransformator::replaceLiteralsToObjectsInExpressionList(ExprListNode * root) {
    for (auto& expr : *root->exprs ) {
        replaceLiteralsToObjectsInExpression(expr);
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInStatement(StmtNode * root) {
    switch (root->type) {
        case _IF_STMT:
            replaceLiteralsToObjectsInExpression(root->cond);
            replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _EXPRESSION:
            replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _WHILE:
            replaceLiteralsToObjectsInExpression(root->cond);
            replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _FOR:
            replaceLiteralsToObjectsInExpression(root->cond);
            replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _DO_WHILE:
            replaceLiteralsToObjectsInExpression(root->cond);
            replaceLiteralsToObjectsInStatementList(root->blockStmts);
            break;
        case _VAR:
            replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _VAL:
            replaceLiteralsToObjectsInExpression(root->expr);
            break;
        case _RETURN:
            replaceLiteralsToObjectsInExpression(root->expr);
            break;
        default: break;
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInStatementList(StmtListNode * root) {
    for (auto& stmt : *root->stmts ) {
        replaceLiteralsToObjectsInStatement(stmt);
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInFunction(FunNode * root) {
    if (root != nullptr) replaceLiteralsToObjectsInStatementList(root->body);
}

void LiteralsTransformator::replaceLiteralsToObjectsInClass(ClassNode * root) {
    /*TODO modifiers*/
    replaceLiteralsToObjectsInInheritance(root->inheritance);
    replaceLiteralsToObjectsInClassBody(root->body);
    replaceLiteralsToObjectsInConstructor(root->primaryConstructor);
}

void LiteralsTransformator::replaceLiteralsToObjectsInInheritance(Inheritance * root) {
    replaceLiteralsToObjectsInExpressionList(root->constructorParams);
}

void LiteralsTransformator::replaceLiteralsToObjectsInClassBody(ClassBodyNode * root) {
    for (auto & constr : *root->constructors) {
        replaceLiteralsToObjectsInConstructor(constr);
    }
    replaceLiteralsToObjectsInStatementList(root->fields);
    for (auto & method : *root->methods) {
        replaceLiteralsToObjectsInFunction(method);
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInConstructor(Constructor * root) {
    /*TODO modifiers prevConstructorType prevConstructorArgs*/
    replaceLiteralsToObjectsInConstructorArgs(root->args);
    replaceLiteralsToObjectsInStatementList(root->stmts);
}

void LiteralsTransformator::replaceLiteralsToObjectsInConstructorArgs(ConstructorArgs * root) {
    for (auto & arg : root->args) {
        if (VarDeclaration * varDecl = dynamic_cast<VarDeclaration*>(arg)) {
            replaceLiteralsToObjectsInVarDeclaration(varDecl);
        } else if (StmtNode * stmt = dynamic_cast<StmtNode*>(arg)) {
            replaceLiteralsToObjectsInStatement(stmt);
        }
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInVarDeclarationList(VarDeclarationList * root) {
    for (auto & decl : *root->decls) {
        replaceLiteralsToObjectsInVarDeclaration(decl);
    }
}

void LiteralsTransformator::replaceLiteralsToObjectsInVarDeclaration(VarDeclaration * root) {
    /*TODO не знаю*/
}