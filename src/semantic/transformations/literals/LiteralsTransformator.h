#ifndef KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#define KOTLIN_COMPILER_LITERALSTRASFORMATOR_H
#include "topLevel/KotlinFileNode.h"


class LiteralsTransformator {
public:
    static void replaceLiteralsToObjects(KotlinFileNode* root);

private:


};


#endif //KOTLIN_COMPILER_LITERALSTRASFORMATOR_H