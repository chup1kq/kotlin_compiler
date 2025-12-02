#include <iostream>

#include "lexer-build/parser.hpp"
#include "src/nodes/topLevel/KotlinFileNode.h"

int main() {
    KotlinFileNode* root = nullptr;

    yyparse(&root);

    if (root != nullptr) {
        std::cout << "AST was created" << std::endl;
    } else {
        std::cout << "Error: root == nullptr" << std::endl;
    }

    return 0;
}
