#include "KotlinElementList.h"

KotlinElementList *KotlinElementList::addElement(KotlinElementList *list, ClassNode *classNode) {
    if (!list) {
        list = new KotlinElementList();
    }

    list->classList.push_back(classNode);
    return list;
}

KotlinElementList *KotlinElementList::addElement(KotlinElementList *list, FunNode *functionNode) {
    if (!list) {
        list = new KotlinElementList();
    }

    list->functionList.push_back(functionNode);
    return list;
}

