#include "KotlinCodeGenerator.h"

#include <cstring>
#include <stdexcept>

#include "BytecodeGenerator.h"

std::vector<uint8_t> KotlinCodeGenerator::generateStatement(StmtNode *stmt, ClassTableElement *classElement,
                                                                 MethodTableElement *methodElement) {
    if (stmt->type == _EXPRESSION) {
        return generateExpression(stmt->expr, classElement, methodElement);
    } else if (stmt->type == _RETURN) {
        return generateReturnStatement(stmt, classElement, methodElement);
    } else if (stmt->type == _WHILE || stmt->type == _DO_WHILE) {
        return generateCycle(stmt, classElement, methodElement);
    } else if (stmt->type == _VAL || stmt->type == _VAR) {
        return generateValOrVar(stmt, classElement, methodElement);
    } else if (stmt->type == _FOR) {
        return generateFor(stmt, classElement, methodElement);
    }
    return {};
}

std::vector<uint8_t> KotlinCodeGenerator::generateExpression(ExprNode *expr, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    if (expr->type == _IDENTIFIER) {
        return generateIdentifier(expr, methodElement);
    } else if (expr->type == _ARRAY_EXPR) {

    } else if (expr->type == _ARRAY_ACCESS) {

    }
}

std::vector<uint8_t> KotlinCodeGenerator::generateReturnStatement(StmtNode *stmt, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;

    std::vector<uint8_t> value = generateExpression(stmt->expr, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, value.data(), value.size());

    std::vector<uint8_t> ret = BytecodeGenerator::areturn();
    BytecodeGenerator::appendToByteArray(&res, ret.data(), ret.size());

    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateCycle(StmtNode *stmt, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;
    std::vector<uint8_t> cond;
    std::vector<uint8_t> body;
    std::vector<uint8_t> tmp;
    std::vector<uint8_t> ifBytes;

    for (auto & bodyStmt: *stmt->blockStmts->stmts) {
        tmp = generateStatement(bodyStmt, classElement, methodElement);
        BytecodeGenerator::appendToByteArray(&body, tmp.data(), tmp.size());
    }

    if (stmt->type == _WHILE) {
        std::vector<uint8_t> goTo = BytecodeGenerator::go_to(body.size());
        BytecodeGenerator::appendToByteArray(&res, goTo.data(), goTo.size());
    }
    BytecodeGenerator::appendToByteArray(&res, body.data(), body.size());

    cond = generateExpression(stmt->cond, classElement, methodElement);
    int cn = classElement->constants->findOrAddConstant(UTF8, "JavaTRL/Boolean");
    int c = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);
    int n = classElement->constants->findOrAddConstant(UTF8, "_ivalue");
    int d = classElement->constants->findOrAddConstant(UTF8, "I");
    int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, n, d);
    int fRef = classElement->constants->findOrAddConstant(FieldRef, "", 0, 0, c, nat);

    std::vector<uint8_t> field = BytecodeGenerator::getfield(fRef);
    BytecodeGenerator::appendToByteArray(&cond, field.data(), field.size());
    BytecodeGenerator::appendToByteArray(&res, cond.data(), cond.size());

    //Смещение
    int shift = -(body.size() + cond.size());

    ifBytes = BytecodeGenerator::if_(IfCommandType::NE, shift);
    BytecodeGenerator::appendToByteArray(&res, ifBytes.data(), ifBytes.size());
    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateFor(StmtNode *stmt, ClassTableElement *classElement,
                                                      MethodTableElement *methodElement) {
    /*TODO*/
}

std::vector<uint8_t> KotlinCodeGenerator::generateValOrVar(StmtNode *stmt, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;

    if (!stmt->expr) {
        res = generateExpression(stmt->expr, classElement, methodElement);
        std::string name = stmt->varDeclaration->varId;
        int vId = methodElement->localVarTable->items[name]->id;
        std::vector<uint8_t> codeGen = BytecodeGenerator::astore(vId);
        BytecodeGenerator::appendToByteArray(&res, codeGen.data(), codeGen.size());
    }

    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateIdentifier(ExprNode *expr, MethodTableElement *methodElement) {
    std::vector<uint8_t> res;
    std::vector<uint8_t> bytes;

    if (methodElement->localVarTable->items.count(expr->identifierName) == 0) {
        throw new std::runtime_error("Unknown identifier: " + expr->identifierName);
    }

    if (methodElement->localVarTable->items[expr->identifierName]->type->className == "I") {
        bytes = BytecodeGenerator::iload(methodElement->localVarTable->items[expr->identifierName]->id);
    } else {
        bytes = BytecodeGenerator::aload(methodElement->localVarTable->items[expr->identifierName]->id);
    }

    BytecodeGenerator::appendToByteArray(&res, bytes.data(), bytes.size());
    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateArrayCreation(ExprNode *expr, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;

    auto dims = getArrayDimensions(expr);

    for (int i = dims.sizes.size() - 1; i >= 0; i--) {
        auto sizeCode = BytecodeGenerator::iconstBipushSipush(dims.sizes[i]);
        BytecodeGenerator::appendToByteArray(&res, sizeCode.data(), sizeCode.size());
    }

    // multianewarray с количеством измерений
    std::string typeDesc = getArrayTypeDescriptor(expr->semanticType);
    int classIndex = classElement->constants->findOrAddConstant(Class, "",
        classElement->constants->findOrAddConstant(UTF8, typeDesc));

    res.push_back(0xC5);  // multianewarray
    auto idxBytes = BytecodeGenerator::intToByteVector(classIndex, 2);
    BytecodeGenerator::appendToByteArray(&res, idxBytes.data(), idxBytes.size());
    res.push_back(static_cast<uint8_t>(dims.sizes.size()));  // dims count

    return res;
}

/* TODO под вопросом */
std::vector<uint8_t> KotlinCodeGenerator::generateArrayAccess(ExprNode *expr, ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;

    // Загрузить ссылку на массив (expr->left)
    auto arrayRef = generateExpression(expr->left, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, arrayRef.data(), arrayRef.size());

    // Загрузить индекс (expr->right)
    auto indexCode = generateExpression(expr->right, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, indexCode.data(), indexCode.size());

    // Выбор инструкции по типу массива
    std::string arrayType = expr->left->semanticType->elementType->className;

    if (arrayType == "I") {
        // int[] → iaload
        auto load = BytecodeGenerator::iaload();
        BytecodeGenerator::appendToByteArray(&res, load.data(), load.size());
    } else if (arrayType == "java/lang/String" || arrayType == "java/lang/Object") {
        // String[] / Object[] → aaload
        auto load = BytecodeGenerator::aaload();
        BytecodeGenerator::appendToByteArray(&res, load.data(), load.size());
    } else {
        // По умолчанию для объектов
        auto load = BytecodeGenerator::aaload();
        BytecodeGenerator::appendToByteArray(&res, load.data(), load.size());
    }

    return res;
}

ArrayDimensions KotlinCodeGenerator::getArrayDimensions(ExprNode *expr) {
    ArrayDimensions dims;

    if (expr->type != _ARRAY_EXPR) {
        throw std::runtime_error("Expr isn't array");
    }

    dims.sizes.push_back(expr->elements->exprs->size());

    if (expr->semanticType->elementType) {
        int maxNestedSize = 0;
        for (auto &elem : *expr->elements->exprs) {
            if (elem->type == _ARRAY_EXPR) {
                auto nestedDims = getArrayDimensions(elem);
                dims.nesting = nestedDims.nesting + 1;
                maxNestedSize = std::max(maxNestedSize, nestedDims.sizes[0]);
            }
        }
        dims.sizes.push_back(maxNestedSize);
    }

    return dims;
}

/* TODO это GPT */
std::string KotlinCodeGenerator::getArrayTypeDescriptor(SemanticType *type) {
    std::string desc;
    SemanticType* t = type;
    while (t) {
        desc += "[";
        t = t->elementType;
    }
    if (t && t->className == "I") desc += "I";
    else if (t) desc += "L" + t->className + ";";
    return desc;
}
