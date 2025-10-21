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
    if (has(lexem)) {
        std::cerr << "Error in modifier. Unexpected " << lexem << std::endl;

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
