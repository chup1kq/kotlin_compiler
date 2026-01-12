#include <iostream>
#include <fstream>

#include "lexer-build/parser.hpp"
#include "src/generation/ClassGeneration.h"
#include "src/nodes/topLevel/KotlinFileNode.h"
#include "src/semantic/tables/class/ClassTable.h"
#include "src/semantic/tables/constant/ConstantTable.h"
#include "src/semantic/transformations/attributing/javaRTLTypesTransformator/JavaRTLTypesTransformator.h"
#include "src/semantic/transformations/attributing/baseTypes/BaseTypesAttributing.h"
#include "src/semantic/transformations/literals/LiteralsTransformator.h"
#include "src/semantic/transformations/operators/OperatorsTransformator.h"

void createDotTree(KotlinFileNode* root, const std::string& filename);
void constantTableToCsv(ConstantTable* constantTable, const std::string& filename);
void classTableToCSV(ClassTable* classTable, const std::string& rootDir);

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

    OperatorsTransformator *operatorsTransformator = new OperatorsTransformator();
    operatorsTransformator->transform(root);

    JavaRTLTypesTransformator *RTLTypesTransformator = new JavaRTLTypesTransformator();
    RTLTypesTransformator->transform(root);

    // LiteralsTransformator *LiteralsTransformation = new LiteralsTransformator();
    // LiteralsTransformation->transform(root);

    BaseTypesAttributing *baseTypesAttributing = new BaseTypesAttributing();
    baseTypesAttributing->transform(root);

    createDotTree(root, "after_transform.txt");

    ClassTable *classTable = new ClassTable("MainKt");
    classTable->buildClassTable(root);

    classTableToCSV(classTable, "tables");

    ClassGeneration *classGeneration = new ClassGeneration(classTable->items["MainKt"]);
    classGeneration->generateClassFile("MainKt");

    return 0;
}
