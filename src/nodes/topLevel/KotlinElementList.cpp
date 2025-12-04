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

string KotlinElementList::toDot() const {
    string dot;

    addDotNode(dot);

    if (!classList.empty()) {
        int i = 0;
        for (const auto *it : classList) {
            addDotChild(dot, it, "class_" + to_string(i));
        }
    }

    if (!functionList.empty()) {
        int i = 0;
        for (const auto *it : classList) {
            addDotChild(dot, it, "class_" + to_string(i));
        }
    }

    return dot;
}

string KotlinElementList::getDotLabel() const {
    return "Top level List";
}


