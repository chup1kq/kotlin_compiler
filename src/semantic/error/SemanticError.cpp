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

SemanticError SemanticError::topLevelMethodAlreayExists(const std::string &name) {
    return SemanticError(
        TOP_LEVEL_METHOD_WITH_THIS_NAME_ALREADY_EXIST,
        "Can not create class '" + name + "'. Top level method '" + name + "' aleady exists"
    );
}