#include "SemanticError.h"
#include <iostream>

#include "SemanticErrorCode.h"

SemanticError::SemanticError(int code, const std::string& message)
    : code(code), message(message) {
}

const char* SemanticError::what() const noexcept {
    return message.c_str();
}

// ---------- factory methods ----------

SemanticError SemanticError::emptyTree() {
    return SemanticError(
        EMPTY_AST_TREE,
        "AST tree is empty"
    );
}

SemanticError SemanticError::redefinition(const std::string &name) {
    return SemanticError(
        1001,
        "Redefinition of '" + name + "'"
    );
}

SemanticError SemanticError::undefinedClass(const std::string& name) {
    return SemanticError(
        1002,
        "Undefined class '" + name + "'"
    );
}

SemanticError SemanticError::invalidInheritance(const std::string &cls, const std::string &parent) {
    return SemanticError(
        1003,
        "Class '" + cls + "' cannot inherit from '" + parent + "'"
    );
}

SemanticError SemanticError::finalClassInheritance(const std::string &cls) {
    return SemanticError(
        1004,
        "Cannot inherit from final class '" + cls + "'"
    );
}

SemanticError SemanticError::methodAlreadyExists(const std::string &name) {
    return SemanticError(
        METHOD_ALREADY_EXISTS,
        "Method '" + name + "' already exists"
    );
}

SemanticError SemanticError::classAlreadyExists(const std::string &name) {
    return SemanticError(
        CLASS_ALREADY_EXISTS,
        "Class '" + name + "' already exists"
    );
}

SemanticError SemanticError::topLevelMethodAlreadyExists(const std::string &name) {
    return SemanticError(
        TOP_LEVEL_METHOD_WITH_THIS_NAME_ALREADY_EXIST,
        "Can not create class '" + name + "'. Top level method '" + name + "' aleady exists"
    );
}

SemanticError SemanticError::constructorAlreadyExists(const std::string &name) {
    return SemanticError(
        CONSTRUCTOR_ALREADY_EXISTS,
        "Constructor '" + name + "' with this params already exists"
    );
}

SemanticError SemanticError::missingReturnValue(const std::string &name) {
    return SemanticError(
        MISSING_RETURN_VALUE,
        "Missing return value in function " + name
    );
}

SemanticError SemanticError::returnFromVoid(const std::string &name) {
    return SemanticError(
            RETURN_FROM_VOID,
            "Void function cannot return a value: " + name
    );
}

SemanticError SemanticError::returnTypeMismatch(const std::string &name) {
    return SemanticError(
        RETURN_TYPE_MISMATCH,
        "Return type mismatch in function " + name
    );
}
SemanticError SemanticError::unallowedAssignment() {
    return SemanticError(
        UNALLOWED_ASSIGNMENT_CONTEXT,
        "Assigments are not allowed in this context. Allowed only on top of expression "
    );
}

SemanticError SemanticError::undefinedVariable(const std::string &name) {
    return SemanticError(
        UNDEFINED_VARIABLE,
        "Local variable '" + name + "' is undefined in this scope"
    );
}

SemanticError SemanticError::uninitializedVariable(const std::string &name) {
    return SemanticError(
        UNINITIALIZED_VARIABLE,
        "Local variable '" + name + "' was not initialized"
    );
}

SemanticError SemanticError::valReassignment(const std::string &name) {
    return SemanticError(
        VAL_REASSIGNMENT,
        "Local variable '" + name + "' was not initialized"
    );
}

SemanticError SemanticError::assignmentTypeMismatch(const std::string &leftName, const std::string &rightName) {
    return SemanticError(
        ASSIGNMENT_TYPES_MISMATCH,
        "Invalid assignments types. '" + leftName + "' = '" + rightName + "'"
    );
}

SemanticError SemanticError::invalidArrayAssignment(const std::string &name) {
    return SemanticError(
        INVALID_ARRAY_ASSIGNMENT,
        "Invalid assignment to array '" + name + "'"
    );
}

SemanticError SemanticError::multivarForLoop(const std::string &name) {
    return SemanticError(
        MULTIVAR_FOR_LOOP,
        "Multivar for loops are not supported in this version '" + name + "'"
    );
}

SemanticError SemanticError::forLoopNotArray(const std::string &type) {
    return SemanticError(
        FOR_LOOP_NOT_ARRAY,
        "Only arrays allowed in for loops. Atcual type '" + type + "'"
    );
}

SemanticError SemanticError::undefinedArrayElementType() {
    return SemanticError(
        UNDEFINED_ARRAY_ELEMENTS_TYPE,
        "Undefined types in array creation"
    );
}

SemanticError SemanticError::conditionNotBoolean(const std::string &name) {
    return SemanticError(
        CONDITION_NOT_BOOLEAN,
        "Condition must be boolean, but actualy '" + name + "'"
    );
}

SemanticError SemanticError::classNotFound(const std::string &className, const std::string &methodName) {
    return SemanticError(
        CLASS_NOT_FOUND,
        "Class '" + className + "' not found for method '" + methodName + "'"
    );
}

SemanticError SemanticError::methodNotFound(const std::string &className, const std::string &methodName) {
    return SemanticError(
        METHOD_NOT_FOUND,
        "Method '" + methodName + "' not found for class '" + className + "'"
    );
}

SemanticError SemanticError::methodCandidateNotFound(const std::string &className, const std::string &methodName, const std::string &paramsDesc) {
    return SemanticError(
        METHOD_CANDIDATE_NOT_FOUND,
        "Not found candidate for desc '" + paramsDesc + "' for method '" + methodName + "' in class '" + className + "'"
    );
}

SemanticError SemanticError::nullSemanticTypeForLeftExpr(const ExprType &type, const std::string &name) {
    return SemanticError(
        NULL_SEMANTIC_TYPE_FOR_LEFT_EXPR,
        "Left expr for expr with type " + exprTypeToString(type) + " is undefined in this scope with name '" + name + "'"
    );
}

SemanticError SemanticError::nullSemanticType(const ExprType &type, const std::string &name) {
    return SemanticError(
        NULL_SEMANTIC_TYPE_FOR_LEFT_EXPR,
        "Expr with type " + exprTypeToString(type) + " and name '" + name + "' has no SematicType"
    );
}

SemanticError SemanticError::notReplaceableTypesInStmtDeclaration(const std::string &varName, const std::string &varType, const std::string &exprType) {
    return SemanticError(
        NOT_REPLACEABLE_TYPES,
        "Not replaceable types in var/val statement '" + varName + "'."
                " Var type '" + varType + "' is not replaceable with default value '" + exprType + "'"
    );
}

SemanticError SemanticError::undeclaredType(const std::string &varName) {
    return SemanticError(
        UNDECLARED_TYPE,
        "Type for variable '" + varName + "' is not declared in this scope"
    );
}

SemanticError SemanticError::differentTypesInArrayCreation(const std::string &firstType, const std::string &incompatibleType) {
    return SemanticError(
        DIFFERENT_TYPES_IN_ARRAY_CREATION,
        "Incompatible types in arrayOf. First element type '" + firstType + "', incompatible with '" + incompatibleType + "'"
    );
}

SemanticError SemanticError::notReplaceableTypesInArrayCreation(const std::string &arrayType, const std::string &elementsType) {
    return SemanticError(
        NOT_REPLACEABLE_TYPES,
        "Not replaceable types in arrayOf. Declared type '" + arrayType + "', elements have type '" + elementsType + "'"
    );
}

SemanticError SemanticError::emptyRange() {
    return SemanticError(
        EMPTY_RANGE,
        "Empty range in for cycle"
    );
}

SemanticError SemanticError::invalidRangeType(const std::string &typeName) {
    return SemanticError(
        INVALID_RANGE_TYPE,
        "Range must contain only numbers. Now contains '" + typeName + "'"
    );
}

SemanticError SemanticError::invalidRangeParams(int start, int end) {
    return SemanticError(
        INVALID_RANGE_PARAMS,
        "Start value '" + std::to_string(start) + "' must be >= end value '" + std::to_string(end) + "'"
    );
}

SemanticError SemanticError::invalidStepValue(int stepValue) {
    return SemanticError(
        INVALID_RANGE_PARAMS,
        "Step value must be > 0, but now '" + std::to_string(stepValue) + "'"
    );
}

SemanticError SemanticError::invalidIteratorType(std::string iterType, std::string elementsRangeType) {
    return SemanticError(
        INVALID_ITERATOR_TYPE,
        "Iterator has different type with range elements. Iterator '" + iterType + "' must be '" + elementsRangeType + "'"
    );
}


