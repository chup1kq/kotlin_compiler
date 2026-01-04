#ifndef KOTLIN_COMPILER_TYPENODE_H
#define KOTLIN_COMPILER_TYPENODE_H
#include "../Node.h"
#include "../types.h"

class TypeNode : public Node {
public:
    Type type;
    std::string customName;
    bool isNull;
    TypeNode* arrayType = nullptr;

    static TypeNode* createType(Type type, bool isNull, std::string customName = "");
    static TypeNode* createArrayType(bool isNull, TypeNode* arrayType);
    static TypeNode* makeNullableType(TypeNode* typeNode);

    string toDot() const override;
    string getDotLabel() const override;
};

#endif //KOTLIN_COMPILER_TYPENODE_H
