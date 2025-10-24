#include "TopLevelDeclarationProcessor.h"

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
        return foundClassKeyword();
    }
    else if (input == "constructor") {
        return foundConstructorKeyword();
    }
    else if (input == "fun") {
        return foundFunKeyword();
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

    this->prevLexems.push_front(lexem);
}

void TopLevelDeclarationProcessor::foundOverrideKeyword() {
    if (hasIncompatibleKeywords("override")) {
        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_front("override");
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
    const std::list<std::string> classIncompatibleKeywords = {
        "class",
        "protected",
        "override"
    };

    const std::list<std::string> accessModifierIncompatibleKeywords = {
        "public",
        "protected",
        "private"
    };

    const std::list<std::string> privateIncompatibleKeywords = {
        "public",
        "protected",
        "private",
        "override"
    };

    const std::list<std::string> overrideIncompatibleKeywords = {
        "private",
        "override"
    };

    const std::list<std::string> inheritanceModifierIncompatibleKeywords = {
        "open",
        "final"
    };

    const std::list<std::string> enumIncompatibleKeywords = {
        "enum",
        "protected",
        "open"
    };

    const std::list<std::string> constructorIncompatibleKeywords = {
        "constructor",
        "override",
        "open",
        "final"
    };

    const std::list<std::string> funIncompatibleKeywords = {
        "fun",
        "enum"
    };

    const std::list<std::string>* incompatibleList = nullptr;

    if (lexem == "class") {
        incompatibleList = &classIncompatibleKeywords;
    }
    else if (lexem == "public" || lexem == "protected") {
        incompatibleList = &accessModifierIncompatibleKeywords;
    }
    else if (lexem == "private") {
        incompatibleList = &privateIncompatibleKeywords;
    }
    else if (lexem == "override") {
        incompatibleList = &overrideIncompatibleKeywords;
    }
    else if (lexem == "open" || lexem == "final") {
        incompatibleList = &inheritanceModifierIncompatibleKeywords;
    }
    else if (lexem == "enum") {
        incompatibleList = &enumIncompatibleKeywords;
    }
    else if (lexem == "constructor") {
        incompatibleList = &constructorIncompatibleKeywords;
    }
    else if (lexem == "fun") {
        incompatibleList = &funIncompatibleKeywords;
    }

    if (incompatibleList != nullptr) {
        for (const auto& word : *incompatibleList) {
            if (has(word)) {
                return word;
            }
        }
    }

    return "";
}

void TopLevelDeclarationProcessor::foundEnumKeyword() {
    if (hasIncompatibleKeywords("enum")) {
        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_front("enum");
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
