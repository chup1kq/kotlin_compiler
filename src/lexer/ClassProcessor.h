#ifndef KOTLIN_COMPILER_PREVIOUSMODIFIER_H
#define KOTLIN_COMPILER_PREVIOUSMODIFIER_H
#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include "../../lexer-build/parser.hpp"

class ClassProcessor {
private:
    std::list<std::string> prevLexems;

    void foundModifierKeyword(const std::string& lexem);
    void foundOverrideKeyword();
    yytokentype foundClassKeyword();

    bool has(const std::string& lexem);
    std::string hasIncompatibleKeyword(const std::string& lexem);

    static bool isModifierKeyword(const std::string& lexem);

    yytokentype combineClassLexem();

public:
    yytokentype processElement(std::string input);

};


#endif //KOTLIN_COMPILER_PREVIOUSMODIFIER_H
