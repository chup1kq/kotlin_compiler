#ifndef KOTLIN_COMPILER_VARDECLARATIONLIST_H
#define KOTLIN_COMPILER_VARDECLARATIONLIST_H
#include "VarDeclaration.h"


class VarDeclarationList {
public:
    std::list<VarDeclaration*> *decls;

    VarDeclarationList(VarDeclaration *first);

    static VarDeclarationList* addVarDeclarationToList(VarDeclarationList *list, VarDeclaration *varDeclaration);

};


#endif //KOTLIN_COMPILER_VARDECLARATIONLIST_H