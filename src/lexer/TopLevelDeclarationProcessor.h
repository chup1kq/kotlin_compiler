#ifndef KOTLIN_COMPILER_PREVIOUSMODIFIER_H
#define KOTLIN_COMPILER_PREVIOUSMODIFIER_H
#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include "../../lexer-build/parser.hpp"

class TopLevelDeclarationProcessor {
private:
    std::list<std::string> prevLexems;

    void foundModifierKeyword(const std::string& lexem);
    void foundOverrideKeyword();
    void foundEnumKeyword();
    yytokentype foundClassKeyword();
    yytokentype foundConstructorKeyword();
    yytokentype foundEndKeyword(const std::string& keyword);

    bool has(const std::string& lexem);
    std::string findIncompatibleKeyword(const std::string& lexem);

    static bool isModifierKeyword(const std::string& lexem);

    yytokentype combineClassLexem();
    yytokentype combineConstructorLexem();
    yytokentype combineEndLexem(const std::string& keyword);

    bool hasIncompatibleKeywords(const std::string& lexem);

    std::string currentLexemToString(const std::string& lexem);

public:
    yytokentype processAppropriateElement(const std::string& input);
    void foundInappropriateLexem(const std::string& input);

};


#endif //KOTLIN_COMPILER_PREVIOUSMODIFIER_H
