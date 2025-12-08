#include "KotlinFileNode.h"

KotlinFileNode::KotlinFileNode(KotlinElementList *list) {
    if (!list) {
        topLevelList = new KotlinElementList();
        return;
    }

    topLevelList = list;
}

string KotlinFileNode::toDot() const {
    string dot;

    dot += "digraph KotlinFile {\n";

    addDotNode(dot);
    addDotChild(dot, topLevelList, "topLevelList");

    dot += "}\n";

    return dot;
}

string KotlinFileNode::getDotLabel() const {
    return "KotlinFile";
}
