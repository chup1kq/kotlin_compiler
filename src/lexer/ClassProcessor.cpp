#include "ClassProcessor.h"

yytokentype ClassProcessor::processElement(std::string input) {
    if (isModifierKeyword(input)) {
        foundModifierKeyword(input);
    }

    return NON;
}

bool ClassProcessor::has(const std::string& lexem) {
    return std::find(this->prevLexems.begin(), this->prevLexems.end(), lexem) != this->prevLexems.end();
}

void ClassProcessor::foundModifierKeyword(std::string lexem) {
    if (hasIncompatibleModifierKeyword(lexem)) {
        std::cerr << "Error in modifier. Modifier " << lexem << " is incompatible with previous modifier." << std::endl;

        this->prevLexems.clear();
        return;
    }

    this->prevLexems.push_front(lexem);
}

bool ClassProcessor::isModifierKeyword(const std::string& lexem) {
    return (lexem == "public" ||
        lexem == "private" ||
        lexem == "protected" ||
        lexem == "internal" ||
        lexem == "open" ||
        lexem == "final"
    );
}

bool ClassProcessor::hasIncompatibleModifierKeyword(const std::string& lexem) {
    if (isAccessModifierKeyword(lexem) && hasAccessModifierKeyword() ) {
        return true;
    }

    if (isInheritanceModifierKeyword(lexem) && hasInheritanceModifierKeyword()) {
        return true;
    }

    return false;
}

bool ClassProcessor::isAccessModifierKeyword(const std::string& lexem) {
    return lexem == "private" || lexem == "protected" || lexem == "public" || lexem == "internal";
}

bool ClassProcessor::hasAccessModifierKeyword() {
    return this->has("private") || this->has("protected") || this->has("public") || this->has("internal");
}

bool ClassProcessor::isInheritanceModifierKeyword(const std::string& lexem) {
    return lexem == "open" || lexem == "final";
}

bool ClassProcessor::hasInheritanceModifierKeyword() {
    return this->has("open") || this->has("final");
}