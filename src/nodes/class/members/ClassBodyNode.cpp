#include "ClassBodyNode.h"

ClassBodyNode* ClassBodyNode::addMember(ClassBodyNode* body, Constructor* constructor) {
    if (!body) {
        body = new ClassBodyNode();
    }

    body->constructors.push_back(constructor);
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

    body->methods.push_back(method);
    return body;
}
