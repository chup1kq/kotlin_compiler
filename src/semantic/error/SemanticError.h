#ifndef KOTLIN_COMPILER_SEMANTIC_ERROR_H
#define KOTLIN_COMPILER_SEMANTIC_ERROR_H

#include <string>
#include <stdexcept>

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
    static SemanticError forLoopNotArray(const std::string &name);
    static SemanticError undefinedArrayElementType();
};

#endif //KOTLIN_COMPILER_SEMANTIC_ERROR_H