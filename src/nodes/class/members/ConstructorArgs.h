#ifndef KOTLIN_COMPILER_CONSTRUCTORARGS_H
#define KOTLIN_COMPILER_CONSTRUCTORARGS_H

#include <vector>
#include "Node.h"
#include "VarDeclaration.h"
#include "VarDeclarationList.h"
#include "StmtNode.h"

class ConstructorArgs : public Node {
public:
    std::vector<Node*> args;

    static ConstructorArgs* addMember(ConstructorArgs* args, VarDeclaration* declaration);
    static ConstructorArgs* addMember(ConstructorArgs* args, StmtNode* stmt);
    static ConstructorArgs* addVarDeclarationList(VarDeclarationList* declarations);

    string toDot() const override;
    string getDotLabel() const override;
};

#endif // KOTLIN_COMPILER_CONSTRUCTORARGS_H
