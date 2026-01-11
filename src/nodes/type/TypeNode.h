#ifndef KOTLIN_COMPILER_TYPENODE_H
#define KOTLIN_COMPILER_TYPENODE_H
#include "../Node.h"
#include "../types.h"

class TypeNode : public Node {
public:
    Type type;
    bool isCustomType;
    std::string customName;
    bool isNullable;
    bool isArray;
    TypeNode* arrayType = nullptr;

    TypeNode* clone() const;

    static TypeNode* createType(Type type, bool isNullable, std::string customName = "");
    static TypeNode* createUndefinedType();
    static TypeNode* createArrayType(bool isNullable, TypeNode* arrayType);
    static TypeNode* makeNullableType(TypeNode* typeNode);

    string toDot() const override;
    string getDotLabel() const override;
};

#endif //KOTLIN_COMPILER_TYPENODE_H
