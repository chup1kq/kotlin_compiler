#include "SemanticError.h"
#include <iostream>

SemanticError::SemanticError(int code, const std::string& message)
    : code(code), message(message) {
}

const char* SemanticError::what() const noexcept {
    return message.c_str();
}

// ---------- factory methods ----------

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