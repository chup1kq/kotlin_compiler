#include "ClassTableElement.h"

#include <iostream>

#include "../semantic/error/SemanticError.h"

ClassTableElement::ClassTableElement()
    : name(0),
      isOpen(0),
      superName(0),
      thisClass(0),
      superClass(0),
      methods(new MethodTable()),
      fields(new FieldTable()),
      constants(new ConstantTable())
{}

void ClassTableElement::addMethodsToTable(std::list<FunNode *> funcList) {
    for (auto& func : funcList) {
        addMethodToTable(func);
    }
}

void ClassTableElement::addMethodToTable(FunNode *func) {
    // Имя  метода
    std::string ident = func->name;

    // Возвращаемое значение
    SemanticType* retVal = new SemanticType(func->type);

    // Параметры метода
    vector<FuncParam*> params;
    if (func->args && func->args->decls) {
        for (auto* arg : *func->args->decls) {
            if (!arg)
                continue;

            params.push_back(new FuncParam(arg->varId, new SemanticType(arg->varType)));
        }
    }

    std::string descriptor = createMethodDescriptor(params, retVal);

    if (this->methods->methods.contains(ident)) {
        if (this->methods->methods[ident].contains(descriptor) )
            throw SemanticError::constructorAlreadyExists(descriptor);
    } else
        this->methods->methods[ident] = std::map<std::string, MethodTableElement*>();

    int methodNameNumber = this->constants->findOrAddConstant(UTF8, ident);
    int methodDescNumber = this->constants->findOrAddConstant(UTF8, descriptor);

    this->methods->methods.find(ident)->second[descriptor] = new MethodTableElement(methodNameNumber, methodDescNumber, ident, descriptor, func->body, retVal, params);
    // тут еще он добавляет в FunctionTable

    for (auto& param : params) {
        // тут еще он добавляет в FunctionTable
        this->methods->methods.find(ident)->second[descriptor]->localVarTable->findOrAddLocalVar("", param->type, 1, 1);
    }
}


std::string ClassTableElement::createVoidMethodDescriptor(vector<FuncParam *> params) {
    std::string desc = addParamsToMethodDescriptor(params);

    desc += "V";
    return desc;
}


std::string ClassTableElement::createMethodDescriptor(vector<FuncParam*> params, SemanticType* returnType) {
    std::string desc = addParamsToMethodDescriptor(params);

    if (!returnType->isArray())
        desc += "L";
    else
        desc += "[L";

    desc += returnType->className;
    desc += ";";

    // TODO удалить cout
    std::cout << desc << std::endl;
    return desc;
}

std::string ClassTableElement::addParamsToMethodDescriptor(vector<FuncParam *> params) {
    std::string desc = "(";

    for (auto* param : params) {
        if (!param->type->isArray())
            desc += "L";
        else
            desc += "[L";

        desc += param->type->className;
        desc += ";";
    }
    desc += ")";

    return desc;
}
