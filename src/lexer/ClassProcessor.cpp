#include "ClassProcessor.h"

yytokentype ClassProcessor::processElement(std::string input) {
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

    return NON;
}

bool ClassProcessor::has(const std::string& lexem) {
    return std::find(this->prevLexems.begin(), this->prevLexems.end(), lexem) != this->prevLexems.end();
}

bool ClassProcessor::isModifierKeyword(const std::string& lexem) {
    return lexem == "public" || lexem == "protected" || lexem == "private" || lexem == "open" || lexem == "final";
}

void ClassProcessor::foundModifierKeyword(const std::string& lexem) {
    std::string incompatibleKeyword = hasIncompatibleKeyword(lexem);
    if (!incompatibleKeyword.empty()) {
        std::cerr << "Error in modifier. Modifier '" << lexem <<"' is incompatible with previous modifier: " << incompatibleKeyword << std::endl;

        this->prevLexems.clear();
    }

    this->prevLexems.push_front(lexem);
}

void ClassProcessor::foundOverrideKeyword() {
    std::string incompatibleKeyword = hasIncompatibleKeyword("override");
    if (!incompatibleKeyword.empty()) {
        std::cerr << "Error in modifier. Modifier 'override' is incompatible with previous modifier: " << incompatibleKeyword << std::endl;

        this->prevLexems.clear();
    }

    this->prevLexems.push_front("override");
}


yytokentype ClassProcessor::foundClassKeyword() {
    std::string incompatibleKeyword = hasIncompatibleKeyword("class");
    if (!incompatibleKeyword.empty()) {
        std::cerr << "Error in modifier. Modifier 'class' is incompatible with previous modifier: " << incompatibleKeyword << std::endl;

        this->prevLexems.clear();
        return NON;
    }

    return combineClassLexem();
}

yytokentype ClassProcessor::combineClassLexem() {
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

std::string ClassProcessor::hasIncompatibleKeyword(const std::string& lexem) {
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

    if (incompatibleList != nullptr) {
        for (const auto& word : *incompatibleList) {
            if (has(word)) {
                return word;
            }
        }
    }

    return "";
}

void ClassProcessor::foundEnumKeyword() {
    std::string incompatibleKeyword = hasIncompatibleKeyword("enum");
    if (!incompatibleKeyword.empty()) {
        std::cerr << "Error in modifier. Modifier 'enum' is incompatible with previous modifier: " << incompatibleKeyword << std::endl;

        this->prevLexems.clear();
    }

    this->prevLexems.push_front("enum");
}
