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
        std::string fullKey = ident + "_" + descriptor;  // ✅ "main_()LUnit;"

        // ✅ УПРОЩЕНО: проверяем полный ключ
        if (this->methods->contains(ident, descriptor)) {
            throw SemanticError::constructorAlreadyExists(descriptor);
        }

        int methodNameNumber = this->constants->findOrAddConstant(UTF8, ident);
        int methodDescNumber = this->constants->findOrAddConstant(UTF8, descriptor);

        // ✅ УПРОЩЕНО: используем addMethod
        this->methods->addMethod(ident, descriptor,
            new MethodTableElement(methodNameNumber, methodDescNumber, ident, descriptor,
                                primaryConstructor->stmts, retVal, params));
    }
}

void ClassTableElement::addDefaultConstructorIfNeeded() const {
    std::string initName = "<init>";
    std::string emptyDesc = "()V";  // Пустой конструктор
    std::string fullKey = initName + "_" + emptyDesc;

    // ✅ УПРОЩЕНО: проверяем полный ключ
    if (this->methods->contains(initName, emptyDesc)) {
        return;
    }

    // Создаем дефолтный конструктор
    SemanticType* unitType = SemanticType::classType("JavaRTL/Unit");
    std::vector<FuncParam*> emptyParams;

    int initNameIdx = this->constants->findOrAddConstant(UTF8, initName);
    int initDescIdx = this->constants->findOrAddConstant(UTF8, emptyDesc);

    // ✅ УПРОЩЕНО: используем addMethod
    this->methods->addMethod(initName, emptyDesc,
        new MethodTableElement(
            initNameIdx,
            initDescIdx,
            initName,
            emptyDesc,
            nullptr,
            unitType,
            emptyParams
        )
    );

    std::cout << "Added default constructor: " << initName << emptyDesc << std::endl;
}

void ClassTableElement::addMethodsToTable(std::list<FunNode *> funcList) {
    for (auto& func : funcList) {
        addMethodToTable(func);
    }
}

void ClassTableElement::addMethodToTable(FunNode *func) {
    // Имя метода
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

    // ✅ ПРОВЕРКА УНИКАЛЬНОСТИ ПО ПОЛНОМУ КЛЮЧУ
    if (this->methods->contains(ident, descriptor)) {
        throw SemanticError::constructorAlreadyExists(descriptor);
    }

    // TODO вот тут костыль, что у всех main будет descriptor = ([Ljava/lang/String)V;
    int methodNameNumber = this->constants->findOrAddConstant(UTF8, ident);
    int methodDescNumber;

    if (func->name == "main") {
        methodDescNumber = this->constants->findOrAddConstant(UTF8, "([Ljava/lang/String;)V");
    }
    else {
        methodDescNumber = this->constants->findOrAddConstant(UTF8, descriptor);
    }


    // ✅ УПРОЩЕНО: используем addMethod
    MethodTableElement* methodElem = new MethodTableElement(
        methodNameNumber, methodDescNumber, ident, descriptor,
        func->body, retVal, params);

    this->methods->addMethod(ident, descriptor, methodElem);

    std::cout << "Вызывается заполением методов в MainKt count = " << ident << " " << descriptor << std::endl;

    // Добавляем переменную this в локальные переменные
    SemanticType* ths = SemanticType::classType(clsName, false);
    ths->classTableElement = this;
    methodElem->localVarTable->findOrAddLocalVar("this", ths, 1, 1);

    // Добавляем параметры метода в локальные переменные
    for (auto& param : params) {
        methodElem->localVarTable->findOrAddLocalVar("", param->type, 1, 1);
    }
}

void ClassTableElement::addStandardMethodToTable(
    const std::string &methodName,
    SemanticType *returnType,
    const std::string &fullDesc
) {
    int methodNameNumber = this->constants->findOrAddConstant(UTF8, methodName);
    int methodDescNumber = this->constants->findOrAddConstant(UTF8, fullDesc);

    MethodTableElement* methodElem = new MethodTableElement(
       methodNameNumber, methodDescNumber, methodName, fullDesc,
       nullptr, returnType, vector<FuncParam*>());

    this->methods->addMethod(methodName, fullDesc, methodElem);
}

void ClassTableElement::addFieldsToTable(StmtListNode *fields) {
    if (!fields || !fields->stmts) {
        return;
    }

    for (auto& field : *fields->stmts) {
        addFieldToTable(field);
    }
}

void ClassTableElement::addFieldToTable(StmtNode *stmt) {
    if (!stmt || (stmt->type != _VAL && stmt->type != _VAR)) {
        return;
    }

    // Название поля
    std::string fieldName = stmt->varDeclaration->varId;

    if (this->fields->contains(fieldName)) {
        throw SemanticError::fieldAlreadyExists(fieldName, this->clsName);
    }

    if (!stmt->varDeclaration->defaultValue) {
        throw SemanticError::fieldNotInitialized(fieldName, this->clsName);
    }

    // Является ли константой
    int isConst = 0;
    if (stmt->type == _VAL) {
        isConst = 1;
    }

    // Тип поля
    if (!stmt->varDeclaration->varType) {
        throw SemanticError::invalidFieldType(fieldName, typeToString(stmt->varDeclaration->varType->type));
    }
    SemanticType* fieldType = new SemanticType(stmt->varDeclaration->varType);

    std::string descriptor = createTypeDescriptor(fieldType);

    int fieldNameNumber = this->constants->findOrAddConstant(UTF8, fieldName);
    int fieldDescNumber = this->constants->findOrAddConstant(UTF8, descriptor);

    FieldTableElement* fieldElem = new FieldTableElement(
        fieldNameNumber, fieldDescNumber, fieldName, descriptor, stmt->varModifiers, isConst, stmt->varDeclaration->defaultValue, fieldType
    );

    this->fields->addField(fieldName, fieldName, fieldElem);
}

std::string ClassTableElement::createVoidMethodDescriptor(vector<FuncParam *> params) {
    std::string desc = addParamsToMethodDescriptor(params);
    desc += "V";
    return desc;
}

std::string ClassTableElement::createMethodDescriptor(vector<FuncParam*> params, SemanticType* returnType) {
    std::string desc = addParamsToMethodDescriptor(params);

    if (returnType->className == "JavaRTL/Unit") {
        desc += "V";
        return desc;
    }

    if (!returnType->isArray()) {
        desc += "L";
        desc += returnType->className;
    }
    else {
        desc += "[L";
        desc += returnType->elementType->className;
    }
    desc += ";";

    std::cout << desc << std::endl;
    return desc;
}

std::string ClassTableElement::createMethodDescriptor(vector<SemanticType*> params, SemanticType* returnType) {
    std::string desc = addParamsToMethodDescriptor(params);

    if (returnType->className == "JavaRTL/Unit") {
        desc += "V";
        return desc;
    }

    if (!returnType->isArray()) {
        desc += "L";
        desc += returnType->className;
    }
    else {
        desc += "[L";
        desc += returnType->elementType->className;
    }
    desc += ";";

    std::cout << desc << std::endl;
    return desc;
}

std::string ClassTableElement::addParamsToMethodDescriptor(vector<FuncParam *> params) {
    std::string desc = "(";

    for (auto* param : params) {
        if (!param->type->isArray()) {
            desc += "L";
            desc += param->type->className;
        }
        else {
            desc += "[L";
            desc += param->type->elementType->className;
        }
        desc += ";";
    }
    desc += ")";

    return desc;
}

std::string ClassTableElement::addParamsToMethodDescriptor(vector<SemanticType *> params) {
    std::string desc = "(";

    for (SemanticType* t : params) {
        if (!t->isArray()) {
            desc += "L";
            desc += t->className;
        }
        else {
            desc += "[L";
            desc += t->elementType->className;
        }
        desc += ";";
    }
    desc += ")";

    return desc;
}

std::string ClassTableElement::createTypeDescriptor(SemanticType *type) {
    std::string desc;
    if (type->isArray()) {
        desc += "[L";
        desc += type->elementType->className;
    }
    else {
        desc += "L";
        desc += type->className;
    }
    desc += ";";

    return desc;
}

void ClassTableElement::checkFieldsModifiers(ClassTableElement* superClass, std::map<std::string, ClassTableElement*> items) {
    for (auto& field : this->fields->fields) {
        checkFiledModifier(field.second, superClass, items);
    }
}

void ClassTableElement::checkFiledModifier(FieldTableElement* field, ClassTableElement* superClass, std::map<std::string, ClassTableElement*> items) {
    ModifierMap* thisFieldMods = field->modifierMap;

    // Если супер класса нет
    if (!superClass) {
        if (thisFieldMods->isOverride()) {
            throw SemanticError::hasNoSuperClassToOverride(this->clsName, field->strName);
        }

        setDefaultModifiers(thisFieldMods);

        return;
    }

    // Тут супер класс есть
    std::string superClassName = getClassNameForSuperField(field, superClass, items);
    FieldTableElement* superField = items[superClassName]->fields->fields.at(field->strName);

    // Если в супер классе (или ранее по цепочке наследования) НЕТ такого поля
    if (!superField) {
        if (thisFieldMods->isOverride()) {
            throw SemanticError::hasNoElementToOverrideInSuperClasses(field->strName, this->clsName, superClassName);
        }

        return;
    }

    // В супер классе (или ранее по цепочке наследования) ЕСТЬ такое поле
    ModifierMap* superFieldMods = superField->modifierMap;

    // Супер поле PRIVATE, то есть оно не видно в наследниках
    if (superFieldMods->isPrivate()) {
        setDefaultModifiers(thisFieldMods);

        return;
    }

    // Супер поле FINAL и его нельзя переопределить или создать такое же (а если мы тут, то мы уже создали такое же, ошибка)
    if (superFieldMods->isFinal()) {
        throw SemanticError::canNotOverrideFinalElement(field->strName, this->clsName, superClassName);
    }

    // Супер поле OPEN, но текущее поле не OVERRIDE
    if (!thisFieldMods->isOverride()) {
        throw SemanticError::elementNeedsOverride(field->strName, this->clsName, superClassName);
    }

    // ----------- Тут супер поле OPEN и текущее поле с OVERRIDE -----------

    // Нельзя ослаблять видимость
    if (thisFieldMods->isProtected() && superFieldMods->isPublic()) {
        throw SemanticError::weakenVisibilityModifier(field->strName, this->clsName, superClassName);
    }

    // Нельзя переопределять тип
    if (!field->fieldType->isReplaceable(*superField->fieldType)) {
        throw SemanticError::notReplaceableTypeInSuperClass(
            field->strName,
            this->clsName,
            field->fieldType->toString(),
            superField->fieldType->toString()
        );
    }

    // Нельзя менять VAL/VAR
    if (field->isConst != superField->isConst) {
        throw SemanticError::changeConsistencyInOverride(field->strName, this->clsName);
    }

    if (thisFieldMods->getVisibility() == NONE) {
        thisFieldMods->modifiers->at("visibility") = superFieldMods->modifiers->at("visibility");
    }
    if (thisFieldMods->getInheritance() == NONE) {
        thisFieldMods->modifiers->at("inheritance") = OPEN;
    }
}

// Рекурсивно получаем поле по имени, идя по цепочке наследования
std::string ClassTableElement::getClassNameForSuperField(FieldTableElement *field, ClassTableElement *superClass, std::map<std::string, ClassTableElement*> items) {
    if (!superClass) {
        return "";
    }

    // Наши в этом наследнике
    if (superClass->fields->fields.contains(field->strName)) {
        return superClass->clsName;
    }

    // Если больше наследника нет
    if (superClass->superClsName.empty()) {
        return "";
    }

    return getClassNameForSuperField(field, items.at(superClass->superClsName), items);
}

void ClassTableElement::setDefaultModifiers(ModifierMap *mods) {
    if (mods->getVisibility() == NONE) {
        mods->modifiers->at("visibility") = PUBLIC;
    }
    if (mods->getInheritance() == NONE) {
        mods->modifiers->at("inheritance") = FINAL;
    }
}
