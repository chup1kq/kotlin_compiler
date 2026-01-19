#ifndef KOTLIN_COMPILER_MODIFIERLIST_H
#define KOTLIN_COMPILER_MODIFIERLIST_H
#include <map>
#include <string>

#include "Node.h"
#include "types.h"


class ModifierMap : public Node  {
public:
    std::map<std::string, ModifierType> *modifiers;

    static ModifierMap* createFunOrVarModifiers(ModifierType visibility, ModifierType inheritance, ModifierType override);
    static ModifierMap* createClassModifiers(ModifierType visibility, ModifierType inheritance);
    static ModifierMap* createClassConstructorModifiers(ModifierType visibility);
    static ModifierMap* createEnumModifiers(ModifierType visibility);
    static ModifierMap* createEnumConstructorModifiers(ModifierType visibility);

    ModifierType getVisibility();
    ModifierType getInheritance();
    ModifierType getOverride();

    bool isOpen();
    bool isFinal();
    bool isPublic();
    bool isProtected();
    bool isPrivate();
    bool isOverride();

    string toDot() const override;
    string getDotLabel() const override;
};


#endif //KOTLIN_COMPILER_MODIFIERLIST_H