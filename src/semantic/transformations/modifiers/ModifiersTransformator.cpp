#include "ModifiersTransformator.h"

#include "../../error/SemanticError.h"

void ModifiersTransformator::transform(KotlinFileNode* root) {
    if (!root || !root->topLevelList)
        throw SemanticError::emptyTree();

    transformTopLevelFunctions(*root->topLevelList->functionList);
}

void ModifiersTransformator::transformTopLevelFunctions(std::list<FunNode *> functions) {
    for (auto& f : functions) {
        transformTopLevelFunction(f);
    }
}

void ModifiersTransformator::transformTopLevelFunction(FunNode *f) {
    if (!f) {
        return;
    }

    ModifierMap* mods = f->modifiers;

    if (mods->getInheritance() != NONE) {
        throw SemanticError::invalidTopLevelModifier(f->name, modifierToString(mods->getInheritance()));
    }
    if (mods->isProtected()) {
        throw SemanticError::invalidTopLevelModifier(f->name, modifierToString(mods->getVisibility()));
    }
    if (mods->isOverride()) {
        throw SemanticError::invalidTopLevelModifier(f->name, modifierToString(mods->getOverride()));
    }

    // Так как не поддерживаются другие файлы, то приводим к public, т.к. разницы нет
    mods->modifiers->at("visibility") = PUBLIC;
}
