#ifndef KOTLIN_COMPILER_KOTLINCODEGENERATOR_H
#define KOTLIN_COMPILER_KOTLINCODEGENERATOR_H
#include <cstdint>
#include <vector>

#include "StmtNode.h"
#include "../semantic/tables/class/ClassTableElement.h"

struct ArrayDimensions {
    std::vector<int> sizes;  // [2, 2] для int[][] {{1},{2,3}}
    int nesting;             // 1
};

class KotlinCodeGenerator {
public:


private:
    static std::vector<uint8_t> generateStatement(StmtNode* stmt, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateExpression(ExprNode* expr, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateReturnStatement(StmtNode* stmt, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateCycle(StmtNode* stmt, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateFor(StmtNode* stmt, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateValOrVar(StmtNode* stmt, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateIdentifier(ExprNode* expr, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateArrayCreation(ExprNode* expr, ClassTableElement* classElement, MethodTableElement* methodElement);
    static std::vector<uint8_t> generateArrayAccess(ExprNode* expr, ClassTableElement* classElement, MethodTableElement* methodElement);

    static ArrayDimensions getArrayDimensions(ExprNode *expr);
    static std::string getArrayTypeDescriptor(SemanticType* type);
};


#endif //KOTLIN_COMPILER_KOTLINCODEGENERATOR_H