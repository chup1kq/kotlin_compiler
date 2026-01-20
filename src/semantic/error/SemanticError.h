#ifndef KOTLIN_COMPILER_SEMANTIC_ERROR_H
#define KOTLIN_COMPILER_SEMANTIC_ERROR_H

#include <string>
#include <stdexcept>

#include "types.h"

class SemanticError : public std::exception {
public:
    int code;
    std::string message;

    SemanticError(int code, const std::string& message);

    const char* what() const noexcept override;

    // ---------- factory methods ----------
    static SemanticError emptyTree();

    static SemanticError redefinition(const std::string &name);
    static SemanticError undefinedClass(const std::string& name);
    static SemanticError invalidInheritance(const std::string &cls, const std::string &parent);
    static SemanticError finalClassInheritance(const std::string &cls);
    static SemanticError methodAlreadyExists(const std::string &desc);
    static SemanticError classAlreadyExists(const std::string &name);
    static SemanticError topLevelMethodAlreadyExists(const std::string &name);
    static SemanticError constructorAlreadyExists(const std::string &name);
    static SemanticError missingReturnValue(const std::string &name);
    static SemanticError returnFromVoid(const std::string &name);
    static SemanticError returnTypeMismatch(const std::string &name);
    static SemanticError unallowedAssignment();
    static SemanticError undefinedVariable(const std::string &name);
    static SemanticError uninitializedVariable(const std::string &name);
    static SemanticError valReassignment(const std::string &name);
    static SemanticError assignmentTypeMismatch(const std::string &leftName, const std::string &rightName);
    static SemanticError invalidArrayAssignment(const std::string &name);
    static SemanticError multivarForLoop(const std::string &name);
    static SemanticError forLoopNotArray(const std::string &type);
    static SemanticError undefinedArrayElementType();
    static SemanticError conditionNotBoolean(const std::string &name);
    static SemanticError classNotFound(const std::string &className, const std::string &methodName);
    static SemanticError methodNotFound(const std::string &className, const std::string &methodName);
    static SemanticError methodCandidateNotFound(const std::string &className, const std::string &methodName, const std::string &paramDesc);
    static SemanticError nullSemanticTypeForLeftExpr(const ExprType &type, const std::string &name = "");
    static SemanticError nullSemanticType(const ExprType &type, const std::string &name = "");
    static SemanticError notReplaceableTypesInStmtDeclaration(const std::string &varName, const std::string &varType, const std::string &exprType);
    static SemanticError undeclaredType(const std::string &varName);
    static SemanticError differentTypesInArrayCreation(const std::string &firstType, const std::string &incompatibleType);
    static SemanticError notReplaceableTypesInArrayCreation(const std::string &arrayType, const std::string &elementsType);
    static SemanticError emptyRange();
    static SemanticError invalidRangeType(const std::string &typeName);
    static SemanticError invalidRangeParams(int start, int end);
    static SemanticError invalidStepValue(int stepValue);
    static SemanticError invalidIteratorType(std::string iterType, std::string elementsRangeType);
    static SemanticError notIdInArrayAccess(const ExprType &type);
    static SemanticError notIntIndexTypeInArrayAccess(const std::string &typeName);
    static SemanticError invalidFieldType(const std::string &fieldName, const std::string &typeName);
    static SemanticError fieldNotInitialized(const std::string &fieldName, const std::string &className);
    static SemanticError fieldAlreadyExists(const std::string &fieldName, const std::string &className);
    static SemanticError invalidTopLevelModifier(const std::string &functionName, const std::string &modifier);
    static SemanticError superClassDoesNotExists(const std::string &thisClass, const std::string &superName);
    static SemanticError inheritanceFromFinalClass(const std::string &thisClass, const std::string &superName);


};

#endif //KOTLIN_COMPILER_SEMANTIC_ERROR_H