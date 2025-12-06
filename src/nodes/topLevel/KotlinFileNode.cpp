#include "KotlinFileNode.h"

KotlinFileNode::KotlinFileNode(KotlinElementList *list) {
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
