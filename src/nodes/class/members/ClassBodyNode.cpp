#include "ClassBodyNode.h"

ClassBodyNode::ClassBodyNode() {
    constructors = new std::list<Constructor*>();
    fields = new StmtListNode();
    methods = new std::list<FunNode*>();
}

ClassBodyNode* ClassBodyNode::addMember(ClassBodyNode* body, Constructor* constructor) {
    if (!body) {
        body = new ClassBodyNode();
    }

    body->constructors->push_back(constructor);
    return body;
}

ClassBodyNode* ClassBodyNode::addMember(ClassBodyNode* body, StmtNode* field) {
    if (!body) {
        body = new ClassBodyNode();
    }

    StmtListNode::addStmtToList(body->fields, field);
    return body;
}

ClassBodyNode* ClassBodyNode::addMember(ClassBodyNode* body, FunNode* method) {
    if (!body) {
        body = new ClassBodyNode();
    }

    body->methods->push_back(method);
    return body;
}

ClassBodyNode* ClassBodyNode::addEnumEntries(ClassBodyNode* body, EnumEntryList* entries) {
    if (!body) {
        body = new ClassBodyNode();
    }

    body->enumEntries = entries;
    return body;
}

string ClassBodyNode::getDotLabel() const {
    return "ClassBodyNode";
}

string ClassBodyNode::toDot() const {
    string dot;

    addDotNode(dot);
    if (!constructors->empty()) {
        int i = 0;
        for (const auto *it : *constructors) {
            addDotChild(dot, it, "constructor_" + to_string(i));
            i++;
        }
    }
    addDotChild(dot, fields, "fields");
    if (!methods->empty()) {
        int i = 0;
        for (const auto *it : *methods) {
            addDotChild(dot, it, "method_" + to_string(i));
            i++;
        }
    }

    // addDotChild(dot, enumEntries, "enum_entries");

    return dot;
}
