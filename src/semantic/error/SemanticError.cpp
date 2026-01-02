#include "SemanticError.h"
#include "SemanticErrorCode.h"
#include <iostream>

SemanticError::SemanticError(int code, std::string message)
    : code(code), message(std::move(message)) {}

void SemanticError::print() const {
    std::cerr << "[Semantic error " << code << "] " << message << std::endl;
}

// ---------- static methods ----------

SemanticError *SemanticError::redefinition(const std::string &name) {
    return new SemanticError(
        REDEFINITION,
        "Redefinition of '" + name + "'"
    );
}

SemanticError *SemanticError::undefinedClass(const std::string &name) {
    return new SemanticError(
        UNDEFINED_CLASS,
        "Undefined class '" + name + "'"
    );
}

SemanticError *SemanticError::invalidInheritance(const std::string &cls, const std::string &parent) {
    return new SemanticError(
        INVALID_INHERITANCE,
        "Class '" + cls + "' cannot inherit from '" + parent + "'"
    );
}

SemanticError *SemanticError::finalClassInheritance(const std::string &cls) {
    return new SemanticError(
        FINAL_CLASS_INHERITANCE,
        "Class '" + cls + "' is final and cannot be inherited"
    );
}
