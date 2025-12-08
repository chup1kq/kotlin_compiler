#ifndef KOTLIN_COMPILER_CLASSBODYNODE_H
#define KOTLIN_COMPILER_CLASSBODYNODE_H

#include <list>
#include "Constructor.h"
#include "FunNode.h"
#include "StmtListNode.h"

#include "enumEntry/EnumEntryList.h"

class ClassBodyNode : public Node {
public:
    std::list<Constructor*> *constructors;
    StmtListNode* fields;
    std::list<FunNode*> *methods;

    ClassBodyNode();

    static ClassBodyNode* addMember(ClassBodyNode* body, Constructor* constructor);
    static ClassBodyNode* addMember(ClassBodyNode* body, StmtNode* field);
    static ClassBodyNode* addMember(ClassBodyNode* body, FunNode* method);

    EnumEntryList* enumEntries;

    static ClassBodyNode* addEnumEntries(ClassBodyNode* body, EnumEntryList* entries);

    string toDot() const override;
    string getDotLabel() const override;
};

#endif // KOTLIN_COMPILER_CLASSBODYNODE_H
