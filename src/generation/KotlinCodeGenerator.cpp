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

}

std::vector<uint8_t> KotlinCodeGenerator::generateValOrVar(StmtNode *stmt, ClassTableElement *classElement,
    MethodTableElement *methodElement) {

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

std::vector<uint8_t> KotlinCodeGenerator::generateBytesForConstantTable(ConstantTable *table) {
    std::vector<uint8_t> res;
    std::vector<uint8_t> bytes;

    for (auto & element: table->items) {
        bytes = generateBytesForConstantTableItem(element.second);
        BytecodeGenerator::appendToByteArray(&res, bytes.data(), bytes.size());
    }

    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateBytesForConstantTableItem(ConstantTableElement *elem) {
    std::vector<uint8_t> res = BytecodeGenerator::intToByteVector(elem->type, 1);

    switch (elem->type) {
        case (UTF8): {
            uint16_t len = elem->utf8String.length();
            std::vector<uint8_t> lenBytes = BytecodeGenerator::intToByteVector(len, 2);
            BytecodeGenerator::appendToByteArray(&res, lenBytes.data(), lenBytes.size());
            const std::string& str = elem->utf8String;
            res.insert(res.end(), str.begin(), str.end());
        } break;
        case (String):
        case (Class): {
            std::vector<uint8_t> ref = BytecodeGenerator::intToByteVector(elem->firstRef, 2);
            BytecodeGenerator::appendToByteArray(&res, ref.data(), ref.size());
        } break;
        case (NameAndType):
        case (MethodRef):
        case (FieldRef): {
            std::vector<uint8_t> firstRef = BytecodeGenerator::intToByteVector(elem->firstRef, 2);
            std::vector<uint8_t> secondRef = BytecodeGenerator::intToByteVector(elem->secondRef, 2);
            BytecodeGenerator::appendToByteArray(&res, firstRef.data(), firstRef.size());
            BytecodeGenerator::appendToByteArray(&res, secondRef.data(), secondRef.size());
        } break;
        case (Integer): {
            std::vector<uint8_t> num = BytecodeGenerator::intToByteVector(elem->intValue, 4);
            BytecodeGenerator::appendToByteArray(&res, num.data(), num.size());
        } break;
        case (Double): {
            std::vector<uint8_t> num = BytecodeGenerator::intToByteVector(elem->doubleValue, 8);
            BytecodeGenerator::appendToByteArray(&res, num.data(), num.size());
        } break;
    }

    return res;
}