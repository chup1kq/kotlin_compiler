#include "JavaRTLTypesTransformator.h"

#include "../../../error/SemanticError.h"

void JavaRTLTypesTransformator::replaceTypesToJavaRTL(KotlinFileNode *root) {
    if (root == nullptr ||
        root->topLevelList == nullptr ||
        (root->topLevelList->classList->empty() &&
        root->topLevelList->functionList->empty())
    ) {
        throw SemanticError::emptyTree();
    }

    replaceToRTLInFunctions(*root->topLevelList->functionList);
    replaceToRTLInClasses(*root->topLevelList->classList);
}

void JavaRTLTypesTransformator::replaceToRTLInClasses(std::list<ClassNode*> classes) {
    for (auto& cls : classes) {
        // TODO добавить обработку полей и конструкторов класса
        if (!cls || !cls->body || !cls->body->methods)
            continue;

        replaceToRTLInFunctions(*cls->body->methods);
    }
}


void JavaRTLTypesTransformator::replaceToRTLInFunctions(std::list<FunNode*> functions) {
    for (auto& function : functions ) {
        if (!function || !function->body || !function->body->stmts)
            continue;

        replaceToRTLInStatements(*function->body->stmts);
    }
}

void JavaRTLTypesTransformator::replaceToRTLInStatements(std::list<StmtNode *> stmts) {
    for (auto& stmt : stmts) {
        if (stmt)
            replaceToRTLInStatement(stmt);
    }
}

void JavaRTLTypesTransformator::replaceToRTLInStatement(StmtNode *stmt) {
    if (!stmt)
        return;

    if (stmt->varDeclaration && stmt->varDeclaration->varType)
        replaceToRTLInTypeNode(stmt->varDeclaration->varType);

    if (stmt->forIterator && stmt->forIterator->varType)
        replaceToRTLInTypeNode(stmt->forIterator->varType);

    if (stmt->forIteratorList && stmt->forIteratorList->decls)
        replaceToRTLInVarDeclarations(*stmt->forIteratorList->decls);
}


void JavaRTLTypesTransformator::replaceToRTLInTypeNode(TypeNode *typeNode) {
    if (typeNode->type == _ARRAY) {
        replaceToRTLInTypeNode(typeNode->arrayType);
        return;
    }

    switch (typeNode->type) {
        case _INT:
            typeNode->customName = "JavaRTL/Int";
            break;
        case _FLOAT:
            typeNode->customName = "JavaRTL/Float";
            break;
        case _DOUBLE:
            typeNode->customName = "JavaRTL/Double";
            break;
        case _CHAR:
            typeNode->customName = "JavaRTL/Char";
            break;
        case _STRING:
            typeNode->customName = "JavaRTL/String";
            break;
        case _BOOLEAN:
            typeNode->customName = "JavaRTL/Boolean";
            break;
        case _UNIT:
            typeNode->customName = "JavaRTL/Unit";
            break;

        default:
            break;
    }

}

void JavaRTLTypesTransformator::replaceToRTLInVarDeclarations(std::list<VarDeclaration *> decls) {
    for (auto& decl : decls) {
        if (decl && decl->varType)
            replaceToRTLInTypeNode(decl->varType);
    }
}
