#include "KotlinCodeGenerator.h"

#include <cstring>
#include <stdexcept>

#include "BytecodeGenerator.h"

std::vector<uint8_t> KotlinCodeGenerator::generate(ClassTableElement *classElement, MethodTableElement *methodElement) {
    return generateMethodAttribute(classElement, methodElement);
}

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

std::vector<uint8_t> KotlinCodeGenerator::generateExpression(
    ExprNode *expr,
    ClassTableElement *classElement,
    MethodTableElement *methodElement
) {
    if (!expr)
        return {};

    switch (expr->type) {

        /* ---------- ЛИТЕРАЛЫ ---------- */
        case _INTEGER_LITERAL:
        case _FLOAT_LITERAL:
        case _DOUBLE_LITERAL:
        case _CHAR_LITERAL:
        case _STRING_LITERAL:
        case _BOOL_LITERAL:
            return generateLiteralCreation(expr, classElement, methodElement);

        /* ---------- ИДЕНТИФИКАТОР ---------- */
        case _IDENTIFIER:
            return generateIdentifier(expr, classElement, methodElement);

        /* ---------- СКОБКИ ---------- */
        case _BRACKETS:
            return generateExpression(expr->left, classElement, methodElement);

        /* ---------- ВЫЗОВЫ ---------- */
        case _FUNC_CALL:
            return generateFuncCall(expr, classElement, methodElement);

        case _FUNC_ACCESS:
            return generateFuncAccess(expr, classElement, methodElement);

        /* ---------- МАССИВЫ ---------- */
        case _ARRAY_EXPR:
            return generateArrayCreation(expr, classElement, methodElement);

        case _ARRAY_ACCESS:
            return generateArrayAccess(expr, classElement, methodElement);

        /* ---------- УНАРНЫЕ ---------- */
        case _UNARY_PLUS:
        case _UNARY_MINUS:
        case _NOT:
        case _PREF_INCREMENT:
        case _PREF_DECREMENT:
        case _POST_INCREMENT:
        case _POST_DECREMENT:
            return generateUnaryExpression(expr, classElement, methodElement);

        /* ---------- БИНАРНЫЕ ---------- */
        case _PLUS:
        case _MINUS:
        case _MUL:
        case _DIV:
        case _MOD:
        case _LESS:
        case _GREAT:
        case _LESS_EQUAL:
        case _GREAT_EQUAL:
        case _EQUAL:
        case _NOT_EQUAL:
        case _DISJUNCTION:
        case _CONJUNCTION:
            return generateBinaryOperation(expr, classElement, methodElement);

        /* ---------- ПРИСВАИВАНИЯ ---------- */
        case _ASSIGNMENT:
        case _PLUS_ASSIGNMENT:
        case _MINUS_ASSIGNMENT:
        case _MUL_ASSIGNMENT:
        case _DIV_ASSIGNMENT:
        case _MOD_ASSIGNMENT:
            return generateAssignment(expr, classElement, methodElement);

        default: return {};
    }
    return {};
}


std::vector<uint8_t> KotlinCodeGenerator::generateFuncAccess(ExprNode *expr, ClassTableElement* classElement, MethodTableElement *methodElement) {
    std::vector<uint8_t> result;

    std::vector<uint8_t> obj = generateIdentifier(expr->left, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&result, obj.data(), obj.size());

    std::string desc = "(";

    if (expr->params->exprs->size() > 0) {
        std::vector<uint8_t> params;

        for (auto& cur : *expr->params->exprs) {
            std::vector<uint8_t> p = generateExpression(cur, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&params, p.data(), p.size());
            // вот тут только одномерный массив, без рекурсии, ибо я не ебу как тут сделать правильно
            if (cur->semanticType->isArray()) {
                desc += "[L";
                desc += cur->semanticType->elementType->className;
            }
            else {
                desc += "L";
                desc += cur->semanticType->className;
            }
            desc += ";";
        }
    }

    desc += ")";

    std::string clsn = expr->left->semanticType->className;

    int cn = classElement->constants->findOrAddConstant(UTF8, clsn);
    int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

    SemanticType* r = ClassTable::items[clsn]->methods->methods[expr->identifierName]->retType;
    if (r->isArray()) {
        desc += "[L";
        desc += r->elementType->className;
    }
    else {
        desc += "L";
        desc += r->className;
    }
    desc += ";";

    int n = classElement->constants->findOrAddConstant(UTF8, expr->identifierName);
    int d = classElement->constants->findOrAddConstant(UTF8, desc);
    int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, n, d);
    int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

    std::vector<uint8_t> iv = BytecodeGenerator::invokevirtual(mref);
    BytecodeGenerator::appendToByteArray(&result, iv.data(), iv.size());

    return result;

}

std::vector<uint8_t> KotlinCodeGenerator::generateFuncCall(
    ExprNode *expr,
    ClassTableElement *classElement,
    MethodTableElement *methodElement
) {
    std::vector<uint8_t> res;

    // 1. Загрузить this
    auto aload0 = BytecodeGenerator::aload(0);
    BytecodeGenerator::appendToByteArray(&res, aload0.data(), aload0.size());

    // 2. Аргументы
    for (auto &arg : *expr->params->exprs) {
        auto a = generateExpression(arg, classElement, methodElement);
        BytecodeGenerator::appendToByteArray(&res, a.data(), a.size());
    }

    // 3. Дескриптор
    std::string desc = "(";
    for (auto &arg : *expr->params->exprs) {
        desc += "L" + arg->semanticType->className + ";";
    }
    desc += ")L" + expr->semanticType->className + ";";

    int cn = classElement->constants->findOrAddConstant(
        UTF8, classElement->clsName
    );
    int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

    int mn = classElement->constants->findOrAddConstant(UTF8, expr->identifierName);
    int md = classElement->constants->findOrAddConstant(UTF8, desc);
    int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
    int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

    auto inv = BytecodeGenerator::invokevirtual(mref);
    BytecodeGenerator::appendToByteArray(&res, inv.data(), inv.size());

    return res;
}


std::vector<uint8_t> KotlinCodeGenerator::generateLiteralCreation(ExprNode *expr, ClassTableElement *classElement, MethodTableElement *methodElement) {
    std::vector<uint8_t> res;
    int clas;
    if (expr->fromLiteral == _FROM_INT) {
        int n = classElement->constants->findOrAddConstant(UTF8, "JavaRTL/Int");
        clas = classElement->constants->findOrAddConstant(Class, "", 0, 0, n);

        if (n == -1) {
            return res;
        }

        res = BytecodeGenerator::_new(clas);
        std::vector<uint8_t> dup = BytecodeGenerator::dup();
        BytecodeGenerator::appendToByteArray(&res, dup.data(), dup.size());

        // Параметры
        if (expr->intValue < -32768 || expr->intValue > 32767) {
            int ic = classElement->constants->findOrAddConstant(Integer, "", expr->intValue);
            std::vector<uint8_t> p = BytecodeGenerator::ldc(ic);
            BytecodeGenerator::appendToByteArray(&res, p.data(), p.size());
        }
        else {
            std::vector<uint8_t> p = BytecodeGenerator::iconstBipushSipush(expr->intValue);
            BytecodeGenerator::appendToByteArray(&res, p.data(), p.size());
        }

        // invokespecial
        int mn = classElement->constants->findOrAddConstant(UTF8, "<init>");
        int md = classElement->constants->findOrAddConstant(UTF8, "(I)V");
        int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
        int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, clas, nat);
        std::vector<uint8_t> is = BytecodeGenerator::invokespecial(mref);
        BytecodeGenerator::appendToByteArray(&res, is.data(), is.size());
    } else if (expr->fromLiteral == _FROM_BOOLEAN) {
        int cn = classElement->constants->findOrAddConstant(UTF8, "JavaRTL/Boolean");
        int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

        res = BytecodeGenerator::_new(cls);
        BytecodeGenerator::appendToByteArray(&res, BytecodeGenerator::dup().data(), 1);

        auto v = BytecodeGenerator::iconstBipushSipush(expr->boolValue ? 1 : 0);
        BytecodeGenerator::appendToByteArray(&res, v.data(), v.size());

        int mn = classElement->constants->findOrAddConstant(UTF8, "<init>");
        int md = classElement->constants->findOrAddConstant(UTF8, "(I)V");
        int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
        int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

        auto is = BytecodeGenerator::invokespecial(mref);
        BytecodeGenerator::appendToByteArray(&res, is.data(), is.size());
    } else if (expr->fromLiteral == _FROM_STRING) {
        int s = classElement->constants->findOrAddConstant(
            String,
            "",
            classElement->constants->findOrAddConstant(UTF8, expr->stringValue)
        );
        auto ldc = BytecodeGenerator::ldc(s);
        BytecodeGenerator::appendToByteArray(&res, ldc.data(), ldc.size());
    } else if (expr->fromLiteral == _FROM_CHAR) {
        int cn = classElement->constants->findOrAddConstant(UTF8, "JavaRTL/Char");
        int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

        res = BytecodeGenerator::_new(cls);

        auto dup = BytecodeGenerator::dup();
        BytecodeGenerator::appendToByteArray(&res, dup.data(), dup.size());

        // char → int
        auto v = BytecodeGenerator::iconstBipushSipush((int)expr->charValue);
        BytecodeGenerator::appendToByteArray(&res, v.data(), v.size());

        int mn = classElement->constants->findOrAddConstant(UTF8, "<init>");
        int md = classElement->constants->findOrAddConstant(UTF8, "(I)V");
        int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
        int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

        auto is = BytecodeGenerator::invokespecial(mref);
        BytecodeGenerator::appendToByteArray(&res, is.data(), is.size());

        return res;
    } else if (expr->fromLiteral == _FROM_FLOAT) {
        int cn = classElement->constants->findOrAddConstant(UTF8, "JavaRTL/Float");
        int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

        res = BytecodeGenerator::_new(cls);

        auto dup = BytecodeGenerator::dup();
        BytecodeGenerator::appendToByteArray(&res, dup.data(), dup.size());

        int fc = classElement->constants->findOrAddConstant(Float, "", 0, expr->floatValue);
        auto ldc = BytecodeGenerator::ldc(fc);
        BytecodeGenerator::appendToByteArray(&res, ldc.data(), ldc.size());

        int mn = classElement->constants->findOrAddConstant(UTF8, "<init>");
        int md = classElement->constants->findOrAddConstant(UTF8, "(F)V");
        int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
        int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

        auto is = BytecodeGenerator::invokespecial(mref);
        BytecodeGenerator::appendToByteArray(&res, is.data(), is.size());

        return res;
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

    for (auto &bodyStmt: *stmt->blockStmts->stmts) {
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
    std::vector<uint8_t> res;

    ExprNode *condExpr = stmt->cond;

    int trueLocal = stmt->forIteratorList->decls->front()->id;
    methodElement->localVarTable->items[stmt->forIteratorList->decls->front()->varId]->id;

    std::string lc = "$";
    lc += stmt->id;
    int local = methodElement->localVarTable->items[lc]->id; // Переменная для for.
    auto init = BytecodeGenerator::iconstBipushSipush(0);
    BytecodeGenerator::appendToByteArray(&res, init.data(), init.size());
    auto loc = BytecodeGenerator::istore(local);
    BytecodeGenerator::appendToByteArray(&res, loc.data(), loc.size());


    std::vector<uint8_t> fill;

    auto arr = generateExpression(condExpr, classElement, methodElement); // Загрузка массива.
    BytecodeGenerator::appendToByteArray(&fill, arr.data(), arr.size());


    // Инициализация локалки.
    auto loa = BytecodeGenerator::iload(local);
    BytecodeGenerator::appendToByteArray(&fill, loa.data(), loa.size());
    auto aa = BytecodeGenerator::aaload();
    auto as = BytecodeGenerator::astore(trueLocal);
    BytecodeGenerator::appendToByteArray(&fill, aa.data(), aa.size());
    BytecodeGenerator::appendToByteArray(&fill, as.data(), as.size());

    // Истинное тело.
    if (stmt->blockStmts != NULL) {
        for (auto &bodyStmt: *stmt->blockStmts->stmts) {
            auto tmp = generateStatement(bodyStmt, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&fill, tmp.data(), tmp.size());
        }
    }

    // Возвращение истинной локалки в массив.

    //auto al = aload(trueLocal);

    // Инкрементация.
    auto l2 = BytecodeGenerator::iload(local);
    BytecodeGenerator::appendToByteArray(&fill, l2.data(), l2.size());
    auto bi = BytecodeGenerator::iconstBipushSipush(1);
    BytecodeGenerator::appendToByteArray(&fill, bi.data(), bi.size());

    auto inc = BytecodeGenerator::iadd();
    BytecodeGenerator::appendToByteArray(&fill, inc.data(), inc.size());
    auto str = BytecodeGenerator::istore(local);
    BytecodeGenerator::appendToByteArray(&fill, str.data(), str.size());


    // Проверка.
    std::vector<uint8_t> cond;
    arr = generateExpression(condExpr, classElement, methodElement); // Загрузка массива.
    BytecodeGenerator::appendToByteArray(&cond, arr.data(), arr.size());
    auto arl = BytecodeGenerator::arraylength();
    BytecodeGenerator::appendToByteArray(&cond, arl.data(), arl.size());
    auto l5 = BytecodeGenerator::iload(local);
    BytecodeGenerator::appendToByteArray(&cond, l5.data(), l5.size());

    int offset = fill.size() + cond.size();
    offset = -offset;

    auto gt = BytecodeGenerator::go_to(fill.size());

    BytecodeGenerator::appendToByteArray(&res, gt.data(), gt.size());
    BytecodeGenerator::appendToByteArray(&res, fill.data(), fill.size());
    BytecodeGenerator::appendToByteArray(&res, cond.data(), cond.size());
    auto if_i = BytecodeGenerator::if_icmp(IfCommandType::NE, offset);
    BytecodeGenerator::appendToByteArray(&res, if_i.data(), if_i.size());

    return res;
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

std::vector<uint8_t> KotlinCodeGenerator::generateIdentifier(ExprNode *expr, ClassTableElement* classElement, MethodTableElement *methodElement) {
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
                                                                classElement->constants->findOrAddConstant(
                                                                    UTF8, typeDesc));

    res.push_back(0xC5); // multianewarray
    auto idxBytes = BytecodeGenerator::intToByteVector(classIndex, 2);
    BytecodeGenerator::appendToByteArray(&res, idxBytes.data(), idxBytes.size());
    res.push_back(static_cast<uint8_t>(dims.sizes.size())); // dims count

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
        for (auto &elem: *expr->elements->exprs) {
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
    SemanticType *t = type;
    while (t) {
        desc += "[";
        t = t->elementType;
    }
    if (t && t->className == "I") desc += "I";
    else if (t) desc += "L" + t->className + ";";
    return desc;
}

std::vector<uint8_t> KotlinCodeGenerator::generateMethodAttribute(ClassTableElement *classElement,
    MethodTableElement *methodElement) {
    std::vector<uint8_t> res;

    // 1. name_index "Code"
    int cd = classElement->constants->findOrAddConstant(UTF8, "Code");
    std::vector<uint8_t> codeNameBytes = BytecodeGenerator::intToByteVector(cd, 2);
    BytecodeGenerator::appendToByteArray(&res, codeNameBytes.data(), codeNameBytes.size());

    // 2. Генерация байткода
    std::vector<uint8_t> codeBytes;
    if (methodElement->start && methodElement->start->stmts) {
        for (auto & stmt : *methodElement->start->stmts) {
            if (!stmt) continue;
            std::vector<uint8_t> bytes = generateStatement(stmt, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&codeBytes, bytes.data(), bytes.size());
        }
    }

    // 3. ОБЯЗАТЕЛЬНО добавляем return для void метода!
    std::vector<uint8_t> ret = BytecodeGenerator::_return();  // 0xB1
    BytecodeGenerator::appendToByteArray(&codeBytes, ret.data(), ret.size());

    // 4. attribute_length = 12 + code_length (JVMS §4.7.3)
    uint32_t attrLength = 12 + codeBytes.size();
    std::vector<uint8_t> lengthBytes = BytecodeGenerator::intToByteVector(attrLength, 4);
    BytecodeGenerator::appendToByteArray(&res, lengthBytes.data(), lengthBytes.size());

    // 5. max_stack
    std::vector<uint8_t> stackSizeBytes = BytecodeGenerator::intToByteVector(50, 2);
    BytecodeGenerator::appendToByteArray(&res, stackSizeBytes.data(), stackSizeBytes.size());

    // 6. max_locals (main=2, остальные=по таблице+1)
    int localsSize = (methodElement->strName == "main") ? 2 : methodElement->localVarTable->items.size() + 1;
    std::vector<uint8_t> localsSizeBytes = BytecodeGenerator::intToByteVector(localsSize, 2);
    BytecodeGenerator::appendToByteArray(&res, localsSizeBytes.data(), localsSizeBytes.size());

    // 7. code_length
    std::vector<uint8_t> codeSizeBytes = BytecodeGenerator::intToByteVector(codeBytes.size(), 4);
    BytecodeGenerator::appendToByteArray(&res, codeSizeBytes.data(), codeSizeBytes.size());

    // 8. code
    BytecodeGenerator::appendToByteArray(&res, codeBytes.data(), codeBytes.size());

    // 9. exception_table_length = 0
    std::vector<uint8_t> exceptionBytes = BytecodeGenerator::intToByteVector(0, 2);
    BytecodeGenerator::appendToByteArray(&res, exceptionBytes.data(), exceptionBytes.size());

    // 10. attributes_count = 0
    std::vector<uint8_t> attrCountBytes = BytecodeGenerator::intToByteVector(0, 2);
    BytecodeGenerator::appendToByteArray(&res, attrCountBytes.data(), attrCountBytes.size());

    return res;
}

std::vector<uint8_t> KotlinCodeGenerator::generateUnaryExpression(
    ExprNode *expr,
    ClassTableElement *classElement,
    MethodTableElement *methodElement
) {
    std::vector<uint8_t> res;

    switch (expr->type) {
        case _UNARY_PLUS:
            return generateExpression(expr->left, classElement, methodElement);

        case _UNARY_MINUS: {
            auto v = generateExpression(expr->left, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&res, v.data(), v.size());

            std::string clsName = expr->left->semanticType->className;

            int cn = classElement->constants->findOrAddConstant(UTF8, clsName);
            int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

            int mn = classElement->constants->findOrAddConstant(UTF8, "neg");
            int md = classElement->constants->findOrAddConstant(
                UTF8, "()L" + clsName + ";"
            );
            int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
            int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

            auto iv = BytecodeGenerator::invokevirtual(mref);
            BytecodeGenerator::appendToByteArray(&res, iv.data(), iv.size());
            return res;
        }
        case _NOT: {
            auto v = generateExpression(expr->left, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&res, v.data(), v.size());

            int cn = classElement->constants->findOrAddConstant(UTF8, "JavaRTL/Boolean");
            int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

            int mn = classElement->constants->findOrAddConstant(UTF8, "not");
            int md = classElement->constants->findOrAddConstant(
                UTF8, "()LJavaRTL/Boolean;"
            );
            int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
            int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

            auto iv = BytecodeGenerator::invokevirtual(mref);
            BytecodeGenerator::appendToByteArray(&res, iv.data(), iv.size());
            return res;
        }
        case _PREF_INCREMENT: {
            // load
            auto load = generateIdentifier(expr->left, classElement, methodElement);
            BytecodeGenerator::appendToByteArray(&res, load.data(), load.size());

            // add 1
            auto one = BytecodeGenerator::iconstBipushSipush(1);
            BytecodeGenerator::appendToByteArray(&res, one.data(), one.size());

            auto add = BytecodeGenerator::iadd();
            BytecodeGenerator::appendToByteArray(&res, add.data(), add.size());

            // store
            int id = methodElement->localVarTable->items[expr->left->identifierName]->id;
            auto store = BytecodeGenerator::istore(id);
            BytecodeGenerator::appendToByteArray(&res, store.data(), store.size());

            // reload (результат выражения)
            auto reload = BytecodeGenerator::iload(id);
            BytecodeGenerator::appendToByteArray(&res, reload.data(), reload.size());

            return res;
        }
        case _POST_INCREMENT: {
            int id = methodElement->localVarTable->items[expr->left->identifierName]->id;

            // load old
            auto old = BytecodeGenerator::iload(id);
            BytecodeGenerator::appendToByteArray(&res, old.data(), old.size());

            // load again
            auto cur = BytecodeGenerator::iload(id);
            BytecodeGenerator::appendToByteArray(&res, cur.data(), cur.size());

            auto one = BytecodeGenerator::iconstBipushSipush(1);
            BytecodeGenerator::appendToByteArray(&res, one.data(), one.size());

            auto add = BytecodeGenerator::iadd();
            BytecodeGenerator::appendToByteArray(&res, add.data(), add.size());

            auto store = BytecodeGenerator::istore(id);
            BytecodeGenerator::appendToByteArray(&res, store.data(), store.size());

            return res;
        }
    }
}

std::vector<uint8_t> KotlinCodeGenerator::generateBinaryOperation(
    ExprNode *expr,
    ClassTableElement *classElement,
    MethodTableElement *methodElement
) {
    std::vector<uint8_t> res;

    // left operand
    auto left = generateExpression(expr->left, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, left.data(), left.size());

    // right operand
    auto right = generateExpression(expr->right, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, right.data(), right.size());

    std::string methodName;

    switch (expr->type) {
        case _PLUS:           methodName = "add"; break;
        case _MINUS:          methodName = "sub"; break;
        case _MUL:            methodName = "mul"; break;
        case _DIV:            methodName = "div"; break;
        case _MOD:            methodName = "mod"; break;

        case _LESS:           methodName = "lt";  break;
        case _GREAT:          methodName = "gt";  break;
        case _LESS_EQUAL:     methodName = "le";  break;
        case _GREAT_EQUAL:    methodName = "ge";  break;
        case _EQUAL:          methodName = "eq";  break;
        case _NOT_EQUAL:      methodName = "ne";  break;

        case _DISJUNCTION:    methodName = "or";  break;
        case _CONJUNCTION:    methodName = "and"; break;

        default:
            throw std::runtime_error("Unsupported binary operator");
    }

    std::string className = expr->left->semanticType->className;

    int cn = classElement->constants->findOrAddConstant(UTF8, className);
    int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

    std::string desc =
        "(L" + expr->right->semanticType->className + ";)"
        "L" + expr->semanticType->className + ";";

    int mn = classElement->constants->findOrAddConstant(UTF8, methodName);
    int md = classElement->constants->findOrAddConstant(UTF8, desc);
    int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
    int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

    auto iv = BytecodeGenerator::invokevirtual(mref);
    BytecodeGenerator::appendToByteArray(&res, iv.data(), iv.size());

    return res;
}


std::vector<uint8_t> KotlinCodeGenerator::generateAssignment(
    ExprNode *expr,
    ClassTableElement *classElement,
    MethodTableElement *methodElement
) {
    std::vector<uint8_t> res;

    // имя переменной
    std::string name = expr->left->identifierName;
    auto var = methodElement->localVarTable->items[name];
    int id = var->id;

    bool isInt = (var->type->className == "I");

    // ===== a = b =====
    if (expr->type == _ASSIGNMENT) {
        auto value = generateExpression(expr->right, classElement, methodElement);
        BytecodeGenerator::appendToByteArray(&res, value.data(), value.size());

        auto store = isInt
            ? BytecodeGenerator::istore(id)
            : BytecodeGenerator::astore(id);
        BytecodeGenerator::appendToByteArray(&res, store.data(), store.size());

        // результат выражения — новое значение
        auto load = isInt
            ? BytecodeGenerator::iload(id)
            : BytecodeGenerator::aload(id);
        BytecodeGenerator::appendToByteArray(&res, load.data(), load.size());

        return res;
    }

    // ===== a += b, a -= b ... =====
    // load a
    auto loadA = isInt
        ? BytecodeGenerator::iload(id)
        : BytecodeGenerator::aload(id);
    BytecodeGenerator::appendToByteArray(&res, loadA.data(), loadA.size());

    // load b
    auto value = generateExpression(expr->right, classElement, methodElement);
    BytecodeGenerator::appendToByteArray(&res, value.data(), value.size());

    // выбрать операцию
    std::string method;
    switch (expr->type) {
        case _PLUS_ASSIGNMENT:  method = "add"; break;
        case _MINUS_ASSIGNMENT: method = "sub"; break;
        case _MUL_ASSIGNMENT:   method = "mul"; break;
        case _DIV_ASSIGNMENT:   method = "div"; break;
        case _MOD_ASSIGNMENT:   method = "mod"; break;
        default:
            method = "add";
    }

    std::string className = var->type->className;

    int cn = classElement->constants->findOrAddConstant(UTF8, className);
    int cls = classElement->constants->findOrAddConstant(Class, "", 0, 0, cn);

    std::string desc =
        "(L" + expr->right->semanticType->className + ";)"
        "L" + className + ";";

    int mn = classElement->constants->findOrAddConstant(UTF8, method);
    int md = classElement->constants->findOrAddConstant(UTF8, desc);
    int nat = classElement->constants->findOrAddConstant(NameAndType, "", 0, 0, mn, md);
    int mref = classElement->constants->findOrAddConstant(MethodRef, "", 0, 0, cls, nat);

    auto iv = BytecodeGenerator::invokevirtual(mref);
    BytecodeGenerator::appendToByteArray(&res, iv.data(), iv.size());

    // store result
    auto store = isInt
        ? BytecodeGenerator::istore(id)
        : BytecodeGenerator::astore(id);
    BytecodeGenerator::appendToByteArray(&res, store.data(), store.size());

    // load result as expression value
    auto reload = isInt
        ? BytecodeGenerator::iload(id)
        : BytecodeGenerator::aload(id);
    BytecodeGenerator::appendToByteArray(&res, reload.data(), reload.size());

    return res;
}

