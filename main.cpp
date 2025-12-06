#include <iostream>
#include <fstream>

#include "lexer-build/parser.hpp"
#include "src/nodes/topLevel/KotlinFileNode.h"

void createDotTree(KotlinFileNode* root, const std::string& filename);

int main() {
    KotlinFileNode* root = nullptr;

    yyparse(&root);

    if (root != nullptr) {
        std::cout << "AST was created\n";
    } else {
        std::cout << "Error: root == nullptr\n";
        return 1;
    }

    createDotTree(root, "ast_tree.txt");

    return 0;
}
