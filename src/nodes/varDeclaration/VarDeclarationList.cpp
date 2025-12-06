#include "VarDeclarationList.h"

VarDeclarationList::VarDeclarationList(VarDeclaration *first) {
    decls = new std::list<VarDeclaration*>();
    decls->push_back(first);
}

VarDeclarationList* VarDeclarationList::addVarDeclarationToList(VarDeclarationList *list, VarDeclaration *varDeclaration) {
    if (list == nullptr) {
        return new VarDeclarationList(varDeclaration);
    }

    list->decls->push_back(varDeclaration);
    return list;
}

string VarDeclarationList::getDotLabel() const {
    return "VarDeclarationList";
}

string VarDeclarationList::toDot() const {
    string dot;

    addDotNode(dot);
    if (!decls->empty()) {
        int i = 0;
        for (const auto *it : *decls) {
            addDotChild(dot, it, "declaration_" + to_string(i));
            i++;
        }
    }

    return dot;
}
