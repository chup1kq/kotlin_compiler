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

    void foundModifierKeyword(std::string lexem);

    bool has(const std::string& lexem);
    static bool isModifierKeyword(const std::string& lexem);

public:
    yytokentype processElement(std::string input);

};


#endif //KOTLIN_COMPILER_PREVIOUSMODIFIER_H