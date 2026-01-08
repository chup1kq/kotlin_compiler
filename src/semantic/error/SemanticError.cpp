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

SemanticError SemanticError::forLoopNotArray(const std::string &name) {
    return SemanticError(
        FOR_LOOP_NOT_ARRAY,
        "Only arrays allowed in for loops '" + name + "'"
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
        "Condition nust be boolean '" + name + "'"
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
