#include "Inheritance.h"

Inheritance *Inheritance::createInheritance(string name, ExprListNode *constructorParams) {
    Inheritance *inheritance = new Inheritance();

    inheritance->name = name;
    inheritance->constructorParams = constructorParams;

    return inheritance;
}