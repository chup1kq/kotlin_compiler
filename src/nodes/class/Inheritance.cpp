#include "Inheritance.h"

Inheritance *Inheritance::createInheritance(string name, ExprListNode *constructorParams) {
    Inheritance *inheritance = new Inheritance();

    inheritance->name = name;
    inheritance->constructorParams = constructorParams;

    return inheritance;
}

string Inheritance::getDotLabel() const {
    return name;
}

string Inheritance::toDot() const {
    string dot;

    addDotNode(dot);
    addDotChild(dot, constructorParams, "constructor");

    return dot;
}
