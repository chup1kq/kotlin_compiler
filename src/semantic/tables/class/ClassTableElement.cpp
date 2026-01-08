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

void ClassTableElement::addPrimaryConstructor(Constructor* primaryConstructor) {
    std::string ident = "<init>"; // Имя конструктора для класса

    SemanticType* retVal = SemanticType::classType(this->clsName);

    // TODO вот тут есть дублирование с методом addMethodToTable, посмотреть, может что-то вынести
    std::vector<FuncParam*> params;
    if (primaryConstructor) {
        if (primaryConstructor->args) {
            for (auto* arg : primaryConstructor->args->args) {
                if (!arg)
                    continue;

                std::string ident;
                SemanticType* type;

                if (auto* v = dynamic_cast<VarDeclaration*>(arg)) {
                    ident = v->varId;
                    type = new SemanticType(v->varType);
                }

                else if (auto* s = dynamic_cast<StmtNode*>(arg)) {
                    // TODO если был передан параметр как поле, то его нужно добавить к полям
                }

                params.push_back(new FuncParam(ident, type));
            }
        }

        std::string descriptor = ClassTableElement::createVoidMethodDescriptor(params);

        if (this->methods->methods.contains(ident)) {
            if (this->methods->methods[ident].contains(descriptor) )
                throw SemanticError::constructorAlreadyExists(descriptor);
        } else
            this->methods->methods[ident] = std::map<std::string, MethodTableElement*>();

        int methodNameNumber = this->constants->findOrAddConstant(UTF8, ident);
        int methodDescNumber = this->constants->findOrAddConstant(UTF8, descriptor);

        this->methods->methods.find(ident)->second[descriptor] = new MethodTableElement(methodNameNumber, methodDescNumber, ident, descriptor, primaryConstructor->stmts, retVal, params);
    }
}

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

    // Добавляем переменную this в локальные переменные
    SemanticType* ths = SemanticType::classType(clsName, false);
    ths->classTableElement = this;
    this->methods->methods.find(ident)->second[descriptor]->localVarTable->findOrAddLocalVar("this", ths, 1, 1);

    // Добавляем параметры метода в локальные переменные
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
