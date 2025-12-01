#include "ClassNode.h"

ClassNode* ClassNode::createClassNode(
        ModifierMap* modifiers,
        std::string name,
        Constructor* primaryConstructor,
        Inheritance* inheritance,
        ClassBodyNode* body
) {
    ClassNode* node = new ClassNode();

    node->isEnum = false;
    node->modifiers = modifiers;
    node->name = name;
    node->primaryConstructor = primaryConstructor;
    node->inheritance = inheritance;
    node->body = body;

    return node;
}

ClassNode *ClassNode::createEnumNode(
        ModifierMap *modifiers,
        std::string name,
        Constructor *primaryConstructor,
        ClassBodyNode *body
) {
    ClassNode *node = new ClassNode();

    node->isEnum = true;
    node->modifiers = modifiers;
    node->name = name;
    node->primaryConstructor = primaryConstructor;
    node->body = body;

    return node;

}
