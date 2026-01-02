#ifndef KOTLIN_COMPILER_SEMANTIC_ERROR_H
#define KOTLIN_COMPILER_SEMANTIC_ERROR_H

#include <string>

class SemanticError {
public:
    int code;
    std::string message;

    SemanticError(int code, std::string message);

    void print() const;

    // ---------- factory methods ----------
    static SemanticError* redefinition(const std::string &name);
    static SemanticError* undefinedClass(const std::string& name);
    static SemanticError* invalidInheritance(const std::string &cls, const std::string &parent);
    static SemanticError* finalClassInheritance(const std::string &cls);
};

#endif //KOTLIN_COMPILER_SEMANTIC_ERROR_H
