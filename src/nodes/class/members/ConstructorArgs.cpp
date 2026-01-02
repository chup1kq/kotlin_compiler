#include "ConstructorArgs.h"

ConstructorArgs* ConstructorArgs::addMember(ConstructorArgs* args, VarDeclaration* declaration) {
    if (!args) {
        args = new ConstructorArgs();
    }

    args->args.push_back(declaration);
    return args;
}

ConstructorArgs* ConstructorArgs::addMember(ConstructorArgs* args, StmtNode* stmt) {
    if (!args) {
        args = new ConstructorArgs();
    }

    args->args.push_back(stmt);
    return args;
}

ConstructorArgs* ConstructorArgs::addVarDeclarationList(VarDeclarationList* declarations) {
    ConstructorArgs* args = new ConstructorArgs();

    if (!declarations || !declarations->decls) {
        return args;
    }

    for (VarDeclaration* decl : *declarations->decls) {
        args->args.push_back(decl);
    }

    return args;
}

string ConstructorArgs::getDotLabel() const {
    return "ConstructorArgs";
}

string ConstructorArgs::toDot() const {
    string dot;

    addDotNode(dot);

    for (size_t i = 0; i < args.size(); ++i) {
        addDotChild(dot, args[i], "arg_" + std::to_string(i));
    }

    return dot;
}
