#include "VarDeclarationList.h"

VarDeclarationList::VarDeclarationList(VarDeclaration *first) {
    decls = new std::list<VarDeclaration*>();
    decls->push_back(first);
}

VarDeclarationList* VarDeclarationList::addStmtToList(VarDeclarationList *list, VarDeclaration *varDeclaration) {
    if (list == nullptr) {
        return new VarDeclarationList(varDeclaration);
    }

    list->decls->push_back(varDeclaration);
    return list;
}
