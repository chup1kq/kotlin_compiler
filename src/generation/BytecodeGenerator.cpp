#include "BytecodeGenerator.h"

void BytecodeGenerator::appendArray(const uint8_t *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        m_buffer.push_back(array[i]);
    }
}

void BytecodeGenerator::appendToByteArray(std::vector<uint8_t>* data, const uint8_t* array, size_t size) {
    for (int i = 0; i < size; i++) {
        data->push_back(array[i]);
    }
}

std::vector<uint8_t> BytecodeGenerator::intToByteVector(int num, int arraySize) {
    std::vector<uint8_t> res(arraySize);
    for (size_t i = 0; i < arraySize; i++) {
        res[arraySize - i - 1] = (num >> (i * 8)) & 0xFF;
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::iconstBipushSipush(int num) {
    std::vector<uint8_t> res;
    if (num == -1) {
        res.push_back(0x02);  // iconst_m1
    }

    else if (num >= 0 && num <= 5) {
        res.push_back(0x03 + num);  // iconst_0..5: 0x03+0=0x03, 0x03+5=0x08
    }
    else if (num >= -128 && num <= 127) {
        res.push_back(0x10);  // bipush
        res.push_back(static_cast<uint8_t>(num));
    }
    else if (num >= -32768 && num <= 32767) {
        res.push_back(0x11);  // sipush
        auto temp = intToByteVector(num, 2);
        res.insert(res.end(), temp.begin(), temp.end());
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::iload(int num) {
    std::vector<uint8_t> res;

    if (num >= 0 && num <= 255) {
        res.push_back(0x15);  // iload
        res.push_back(static_cast<uint8_t>(num));
    } else if (num >= 256 && num <= 65535) {
        res.push_back(0xC4);  // wide
        res.push_back(0x15);  // iload
        res.push_back((num >> 8) & 0xFF);
        res.push_back(num & 0xFF);
    } else {
        throw std::runtime_error("iload: Invalid index");
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::aload(int num) {
    std::vector<uint8_t> res;

    if (num >= 0 && num <= 255) {
        res.push_back(0x19);  // aload
        res.push_back(static_cast<uint8_t>(num));
    } else if (num >= 256 && num <= 65535) {
        res.push_back(0xC4);  // wide
        res.push_back(0x19);  // aload
        res.push_back((num >> 8) & 0xFF);
        res.push_back(num & 0xFF);
    } else {
        throw std::runtime_error("aload: Invalid index");
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::istore(int num) {
    std::vector<uint8_t> res;

    if (num >= 0 && num <= 255) {
        res.push_back(0x36);  // istore
        res.push_back(static_cast<uint8_t>(num));
    } else if (num >= 256 && num <= 65535) {
        res.push_back(0xC4);  // wide
        res.push_back(0x36);  // istore
        res.push_back((num >> 8) & 0xFF);
        res.push_back(num & 0xFF);
    } else {
        throw std::runtime_error("istore: Invalid index");
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::astore(int num) {
    std::vector<uint8_t> res;

    if (num >= 0 && num <= 255) {
        res.push_back(0x3A);  // astore
        res.push_back(static_cast<uint8_t>(num));
    } else if (num >= 256 && num <= 65535) {
        res.push_back(0xC4);  // wide
        res.push_back(0x3A);  // astore
        res.push_back((num >> 8) & 0xFF);
        res.push_back(num & 0xFF);
    } else {
        throw std::runtime_error("astore: Invalid index");
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::ldc(int num) {
    std::vector<uint8_t> res;

    if (num >= 1 && num <= 255) {
        res.push_back(0x12);  // ldc
        res.push_back(static_cast<uint8_t>(num));
    } else if (num >= 256 && num <= 65535) {
        res.push_back(0x13);  // ldc_w
        res.push_back((num >> 8) & 0xFF);
        res.push_back(num & 0xFF);
    } else {
        throw std::runtime_error("ldc: Invalid pool index");
    }
    return res;
}

std::vector<uint8_t> BytecodeGenerator::pop() {
    return {0x57};
}

std::vector<uint8_t> BytecodeGenerator::dup() {
    return {0x59};
}

std::vector<uint8_t> BytecodeGenerator::dup2() {
    return {0x5C};
}

std::vector<uint8_t> BytecodeGenerator::go_to(int offset) {
    std::vector<uint8_t> res;
    res.push_back(0xA7);  // goto

    auto bytes = intToByteVector(offset, 2); // signed 16-bit, big-endian
    res.insert(res.end(), bytes.begin(), bytes.end());

    return res;
}

std::vector<uint8_t> BytecodeGenerator::if_icmp(IfCommandType type, int offset) {
    std::vector<uint8_t> res;

    switch (type) {
        case IfCommandType::EQ: res.push_back(0x9F); break; // if_icmpeq
        case IfCommandType::NE: res.push_back(0xA0); break; // if_icmpne
        case IfCommandType::LT: res.push_back(0xA1); break; // if_icmplt
        case IfCommandType::LE: res.push_back(0xA4); break; // if_icmple
        case IfCommandType::GT: res.push_back(0xA3); break; // if_icmpgt
        case IfCommandType::GE: res.push_back(0xA2); break; // if_icmpge
        default:
            throw std::runtime_error("Invalid if_icmp type");
    }

    // signed 16-bit offset от адреса самой инструкции if_icmp
    auto bytes = intToByteVector(offset, 2);
    res.insert(res.end(), bytes.begin(), bytes.end());

    return res;
}

std::vector<uint8_t> BytecodeGenerator::if_acmp(IfCommandType type, int offset) {
    std::vector<uint8_t> res;
    switch (type) {
        case IfCommandType::EQ: res.push_back(0xA5); break; // if_acmpeq
        case IfCommandType::NE: res.push_back(0xA6); break; // if_acmpne
        default: throw std::runtime_error("Invalid if_acmp type");
    }
    auto bytes = intToByteVector(offset, 2);
    res.insert(res.end(), bytes.begin(), bytes.end());
    return res;
}


std::vector<uint8_t> BytecodeGenerator::if_(IfCommandType type, int offset) {
    std::vector<uint8_t> res;

    switch (type) {
        case IfCommandType::EQ: res.push_back(0x99); break; // ifeq
        case IfCommandType::NE: res.push_back(0x9A); break; // ifne
        case IfCommandType::LT: res.push_back(0x9B); break; // iflt
        case IfCommandType::GE: res.push_back(0x9C); break; // ifge
        case IfCommandType::GT: res.push_back(0x9D); break; // ifgt
        case IfCommandType::LE: res.push_back(0x9E); break; // ifle
        default: throw std::runtime_error("Invalid if_ type");
    }

    // signed 16-bit offset (big-endian)
    std::vector<uint8_t> offsetBytes = intToByteVector(offset, 2);
    res.insert(res.end(), offsetBytes.begin(), offsetBytes.end());

    return res;
}

std::vector<uint8_t> BytecodeGenerator::invokespecial(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xB7);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::invokevirtual(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xB6);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::invokestatic(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xB8);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::ireturn() {
    return {0xAC};
}

std::vector<uint8_t> BytecodeGenerator::areturn() {
    return {0xB0};
}

std::vector<uint8_t> BytecodeGenerator::_return() {
    return {0xB1};
}

std::vector<uint8_t> BytecodeGenerator::_new(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xBB);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::getfield(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xB4);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::putfield(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xB5);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::anewarray(int constant) {
    std::vector<uint8_t> res;
    res.push_back(0xBD);

    std::vector<uint8_t> temp = intToByteVector(constant, 2);
    res.insert(res.end(), temp.begin(), temp.end());
    return res;
}

std::vector<uint8_t> BytecodeGenerator::arraylength() {
    return {0xBE};
}

std::vector<uint8_t> BytecodeGenerator::aastore() {
    return {0x53};
}

std::vector<uint8_t> BytecodeGenerator::aaload() {
    return {0x32};
}

std::vector<uint8_t> BytecodeGenerator::iadd() {
    return {0x60};
}

std::vector<uint8_t> BytecodeGenerator::imul() {
    return {0x64};
}

std::vector<uint8_t> BytecodeGenerator::isub() {
    return {0x68};
}

std::vector<uint8_t> BytecodeGenerator::idiv() {
    return {0x6C};
}

std::vector<uint8_t> BytecodeGenerator::iinc(int varIndex, int increment) {
    std::vector<uint8_t> res;
    if (varIndex >= 0 && varIndex <= 255 && increment >= -128 && increment <= 127) {
        res.push_back(0x84);  // iinc
        res.push_back(static_cast<uint8_t>(varIndex));
        res.push_back(static_cast<int8_t>(increment));  // s1
    } else {
        // wide iinc для больших индексов/приращений
        res.push_back(0xC4);  // wide
        res.push_back(0x84);  // iinc
        res.push_back((varIndex >> 8) & 0xFF);
        res.push_back(varIndex & 0xFF);
        res.push_back(static_cast<int8_t>(increment));
    }
    return res;
}


std::vector<uint8_t> BytecodeGenerator::iaload() {
    return {0x46};
}

std::vector<uint8_t> BytecodeGenerator::daload() {
    return {0x4F};
}

std::vector<uint8_t> BytecodeGenerator::faload() {
    return {0x47};
}

std::vector<uint8_t> BytecodeGenerator::laload() {
    return {0x4E};
}

std::vector<uint8_t> BytecodeGenerator::baload() {
    return {0x33};
}

std::vector<uint8_t> BytecodeGenerator::caload() {
    return {0x34};
}

std::vector<uint8_t> BytecodeGenerator::saload() {
    return {0x35};
}

std::vector<uint8_t> BytecodeGenerator::generateBytesForConstantTable(ConstantTable *table) {
    std::vector<uint8_t> res;
    std::vector<uint8_t> bytes;

    for (auto & element: table->items) {
        bytes = generateBytesForConstantTableItem(element.second);
        appendToByteArray(&res, bytes.data(), bytes.size());
    }

    return res;
}

std::vector<uint8_t> BytecodeGenerator::generateBytesForConstantTableItem(ConstantTableElement *elem) {
    std::vector<uint8_t> res = intToByteVector(elem->type, 1);

    switch (elem->type) {
        case (UTF8): {
            uint16_t len = elem->utf8String.length();
            std::vector<uint8_t> lenBytes = intToByteVector(len, 2);
            appendToByteArray(&res, lenBytes.data(), lenBytes.size());
            const std::string& str = elem->utf8String;
            res.insert(res.end(), str.begin(), str.end());
        } break;
        case (String):
        case (Class): {
            std::vector<uint8_t> ref = intToByteVector(elem->firstRef, 2);
            appendToByteArray(&res, ref.data(), ref.size());
        } break;
        case (NameAndType):
        case (MethodRef):
        case (FieldRef): {
            std::vector<uint8_t> firstRef = intToByteVector(elem->firstRef, 2);
            std::vector<uint8_t> secondRef = intToByteVector(elem->secondRef, 2);
            appendToByteArray(&res, firstRef.data(), firstRef.size());
            appendToByteArray(&res, secondRef.data(), secondRef.size());
        } break;
        case (Integer): {
            std::vector<uint8_t> num = intToByteVector(elem->intValue, 4);
            appendToByteArray(&res, num.data(), num.size());
        } break;
        case (Float): {
            std::vector<uint8_t> num = intToByteVector(elem->doubleValue, 8);
            appendToByteArray(&res, num.data(), num.size());
        } break;
    }

    return res;
}