#include "Node.h"

unsigned int Node::maxId = 0;

void Node::addDotNode(string &dot) const {
    dot += to_string(id) + " [label=\"" + getDotLabel() + "\"];\n";
}

void Node::addDotChild(string &dot, const Node *child, const string &arrowLabel) const {
    if (!child) {
        return;
    }

    dot += to_string(id) + " -> " + child->getDotLabel();

    if (!arrowLabel.empty()) {
        dot += " [label=\"" + arrowLabel + "\"]";
    }
    dot += ";\n" + child->toDot();
}


