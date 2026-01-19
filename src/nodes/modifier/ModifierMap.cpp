#include "ModifierMap.h"

ModifierMap* ModifierMap::createFunOrVarModifiers(ModifierType visibility,
                                             ModifierType inheritance,
                                             ModifierType overrideModifier) {
    ModifierMap *map = new ModifierMap();
    map->modifiers = new std::map<std::string, ModifierType>();

    (*map->modifiers)["visibility"] = visibility;
    (*map->modifiers)["inheritance"] = inheritance;
    (*map->modifiers)["override"] = overrideModifier;

    return map;
}

ModifierMap* ModifierMap::createClassModifiers(ModifierType visibility, ModifierType inheritance) {
    ModifierMap *map = new ModifierMap();
    map->modifiers = new std::map<std::string, ModifierType>();

    (*map->modifiers)["visibility"] = visibility;
    (*map->modifiers)["inheritance"] = inheritance;
    (*map->modifiers)["override"] = NONE;

    return map;
}

ModifierMap* ModifierMap::createClassConstructorModifiers(ModifierType visibility) {
    ModifierMap *map = new ModifierMap();
    map->modifiers = new std::map<std::string, ModifierType>();

    (*map->modifiers)["visibility"] = visibility;
    (*map->modifiers)["inheritance"] = NONE;
    (*map->modifiers)["override"] = NONE;

    return map;
}

ModifierType ModifierMap::getVisibility() {
    return this->modifiers->at("visibility");
}

ModifierType ModifierMap::getInheritance() {
    return this->modifiers->at("inheritance");
}

ModifierType ModifierMap::getOverride() {
    return this->modifiers->at("override");
}

bool ModifierMap::isOpen() {
    return this->modifiers->at("inheritance") == OPEN;
}

bool ModifierMap::isFinal() {
    return this->modifiers->at("inheritance") == FINAL;
}

bool ModifierMap::isPrivate() {
    return this->modifiers->at("visibility") == PRIVATE;
}

bool ModifierMap::isProtected() {
    return this->modifiers->at("visibility") == PROTECTED;
}

bool ModifierMap::isPublic() {
    return this->modifiers->at("visibility") == PUBLIC;
}

bool ModifierMap::isOverride() {
    return this->modifiers->at("override") == OVERRIDE;
}

ModifierMap* ModifierMap::createEnumModifiers(ModifierType visibility) {
    return createClassConstructorModifiers(visibility);
}

ModifierMap* ModifierMap::createEnumConstructorModifiers(ModifierType visibility) {
    return createClassConstructorModifiers(visibility);
}

string ModifierMap::getDotLabel() const {
    string dot;

    dot += "visibility: " + modifierToString((*modifiers)["visibility"]);
    dot += "\ninheritance: " + modifierToString((*modifiers)["inheritance"]);
    dot += "\noverride: " + modifierToString((*modifiers)["override"]);

    return dot;
}


string ModifierMap::toDot() const {
    string dot = "";

    addDotNode(dot);

    return dot;
}