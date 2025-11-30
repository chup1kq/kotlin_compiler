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

    return map;
}

ModifierMap* ModifierMap::createClassConstructorModifiers(ModifierType visibility) {
    ModifierMap *map = new ModifierMap();
    map->modifiers = new std::map<std::string, ModifierType>();

    (*map->modifiers)["visibility"] = visibility;

    return map;
}

ModifierMap* ModifierMap::createEnumModifiers(ModifierType visibility) {
    return createClassConstructorModifiers(visibility);
}

ModifierMap* ModifierMap::createEnumConstructorModifiers(ModifierType visibility) {
    return createClassConstructorModifiers(visibility);
}