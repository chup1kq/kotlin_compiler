#ifndef KOTLIN_COMPILER_MODIFIERSTRANSFORMATOR_H
#define KOTLIN_COMPILER_MODIFIERSTRANSFORMATOR_H
#include "topLevel/KotlinFileNode.h"


class ModifiersTransformator {
public:
    void transform(KotlinFileNode* root);

private:
    void transformTopLevelFunctions(std::list<FunNode*> functions);
    void transformTopLevelFunction(FunNode* f);

    void transformClasses(std::list<ClassNode*> classes);
    void transformClass(ClassNode* cls);

    void checkOverrideModifiers(ClassNode* cls);

};


#endif //KOTLIN_COMPILER_MODIFIERSTRANSFORMATOR_H