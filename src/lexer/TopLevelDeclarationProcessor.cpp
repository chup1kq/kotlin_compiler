#include "TopLevelDeclarationProcessor.h"

#include <unordered_map>
#include <vector>

yytokentype TopLevelDeclarationProcessor::processAppropriateElement(const std::string& input) {
    if (isModifierKeyword(input)) {
        foundModifierKeyword(input);
    }
    else if (input == "override") {
        foundOverrideKeyword();
    }
    else if (input == "enum") {
        foundEnumKeyword();
    }
    else if (input == "class") {
        yytokentype returnLexem = foundClassKeyword();
        this->prevLexems.clear();
        return returnLexem;
    }
    else if (input == "constructor") {
        yytokentype returnLexem = foundConstructorKeyword();
        this->prevLexems.clear();
        return returnLexem;
    }
    else if (input == "fun") {
        yytokentype returnLexem = foundFunKeyword();
        this->prevLexems.clear();
        return returnLexem;
    }

    return NON;
}

bool TopLevelDeclarationProcessor::has(const std::string& lexem) {
    return std::find(this->prevLexems.begin(), this->prevLexems.end(), lexem) != this->prevLexems.end();
}

bool TopLevelDeclarationProcessor::isModifierKeyword(const std::string& lexem) {
    return lexem == "public" || lexem == "protected" || lexem == "private" || lexem == "open" || lexem == "final";
}

void TopLevelDeclarationProcessor::foundModifierKeyword(const std::string& lexem) {
    if (hasIncompatibleKeywords(lexem)) {
        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_back(lexem);
}

void TopLevelDeclarationProcessor::foundOverrideKeyword() {
    if (hasIncompatibleKeywords("override")) {
        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_back("override");
}


yytokentype TopLevelDeclarationProcessor::foundClassKeyword() {
    if (hasIncompatibleKeywords("class")) {
        this->prevLexems.clear();
        return NON;
    }

    std::cout << currentLexemToString("class") << std::endl;
    return combineClassLexem();
}

yytokentype TopLevelDeclarationProcessor::combineClassLexem() {
    if (has("enum")) {
        if (has("private")) {
            return PRIVATE_ENUM;
        }
        return PUBLIC_ENUM;
    }

    yytokentype returnValue = NON;

    if (
        (has("private") && has("final")) ||
        (has("private") && !has("open"))
    ) {
        returnValue = PRIVATE_FINAL_CLASS;
    }
    else if (has("private") && has("open")) {
        returnValue = PRIVATE_OPEN_CLASS;
    }
    else if (
        (has("public") && has("open")) ||
        has("open")
    ) {
        returnValue = PUBLIC_OPEN_CLASS;
    }
    else {
        returnValue = PUBLIC_FINAL_CLASS;
    }

    prevLexems.clear();

    return returnValue;
}

std::string TopLevelDeclarationProcessor::findIncompatibleKeyword(const std::string& lexem) {
    static const std::unordered_map<std::string, std::vector<std::string>> incompatibleMap = {
        { "class",      { "protected", "override" } },
        { "public",     { "public", "protected", "private" } },
        { "protected",  { "public", "protected", "private" } },
        { "private",    { "public", "protected", "private", "override" } },
        { "override",   { "private", "override" } },
        { "open",       { "open", "final" } },
        { "final",      { "open", "final" } },
        { "enum",       { "enum", "protected", "open" } },
        { "constructor",{ "override", "open", "final" } },
        { "fun",        { "enum" } }
    };

    auto it = incompatibleMap.find(lexem);
    if (it == incompatibleMap.end()) {
        return "";
    }

    for (const auto& word : it->second) {
        if (has(word)) {
            return word;
        }
    }

    return "";
}

void TopLevelDeclarationProcessor::foundEnumKeyword() {
    if (hasIncompatibleKeywords("enum")) {
        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_back("enum");
}

yytokentype TopLevelDeclarationProcessor::foundConstructorKeyword() {
    if (hasIncompatibleKeywords("constructor")) {
        this->prevLexems.clear();
        return NON;
    }

    std::cout << currentLexemToString("constructor") << std::endl;
    return combineConstructorLexem();
}

yytokentype TopLevelDeclarationProcessor::combineConstructorLexem() {
    if (has("private")) {
        return PRIVATE_CONSTRUCTOR;
    }
    if (has("protected")) {
        return PROTECTED_CONSTRUCTOR;
    }

    return PUBLIC_CONSTRUCTOR;
}

yytokentype TopLevelDeclarationProcessor::foundFunKeyword() {
    if (hasIncompatibleKeywords("fun")) {
        this->prevLexems.clear();
        return NON;
    }

    if (has("private") && has("open")) {
        std::cerr << "Error in modifier. Modifier 'private' is incompatible with 'open' in function declaration." << std::endl;

        this->prevLexems.clear();
        return NON;
    }

    std::cout << currentLexemToString("fun") << std::endl;
    return combineFunLexem();
}

yytokentype TopLevelDeclarationProcessor::combineFunLexem() {
    if (has("override")) {
        if (has("protected") && has("final")) {
            return PROTECTED_FINAL_OVERRIDE_FUN;
        }
        if (has("protected")) {
            return PROTECTED_OPEN_OVERRIDE_FUN;
        }

        if (has("public") && has("final")) {
            return PUBLIC_FINAL_OVERRIDE_FUN;
        }
        if (has("public")) {
            return PUBLIC_OPEN_OVERRIDE_FUN;
        }
        if (has("final")) {
            return FINAL_OVERRIDE_FUN;
        }
        return OPEN_OVERRIDE_FUN;
    }

    if (has("open")) {
        if (has("protected")) {
            return PROTECTED_OPEN_FUN;
        }
        return PUBLIC_OPEN_FUN;
    }

    if (has("final")) {
        if (has("private")) {
            return PRIVATE_FINAL_FUN;
        }
        if (has("protected")) {
            return PROTECTED_FINAL_FUN;
        }
        return PUBLIC_FINAL_FUN;
    }

    if (has("private")) {
        return PRIVATE_FUN;
    }

    return PUBLIC_FUN;
}

std::string TopLevelDeclarationProcessor::currentLexemToString(const std::string& lexem) {
    std::string toPrint;

    for (const auto& l : prevLexems) {
        toPrint += l + " ";
    }

    toPrint += lexem;
    return toPrint;
}

bool TopLevelDeclarationProcessor::hasIncompatibleKeywords(const std::string &lexem) {
    std::string incompatibleKeyword = findIncompatibleKeyword(lexem);
    if (!incompatibleKeyword.empty()) {
        std::cerr << "Error in modifier. Keyword '" << lexem <<"' is incompatible with previous modifier: " << incompatibleKeyword << std::endl;

        return true;
    }

    return false;
}

void TopLevelDeclarationProcessor::foundInappropriateLexem(const std::string& input) {
    if (!this->prevLexems.empty()) {
        std::cerr << "Error in modifier. Expecting top level keyword after '" << currentLexemToString("") << "', but found: " << input << std::endl;

        this->prevLexems.clear();
    }
}
