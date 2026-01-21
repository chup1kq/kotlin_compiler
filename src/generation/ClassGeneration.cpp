#include "ClassGeneration.h"

#include <filesystem>
#include <iostream>

#include "BytecodeGenerator.h"
#include "KotlinCodeGenerator.h"

ClassGeneration::ClassGeneration(ClassTable* cls)
    : m_classes(cls) {
}

uint16_t getMethodAccessFlags(MethodTableElement* method) {
    // <init> — НИКОГДА НЕ STATIC
    if (method->strName == "<init>") {
        return 0x0001; // ACC_PUBLIC
    }

    // main — public static
    if (method->strName == "main") {
        return 0x0009; // ACC_PUBLIC | ACC_STATIC
    }

    // обычные методы
    return 0x0001; // ACC_PUBLIC
}

void ClassGeneration::generateAllClassesFiles() {
    for (auto cls : m_classes->items) {
        if (m_classes->isBuiltinClass(cls.first)) {
            continue;
        }

        generateClassFile(cls.second);
    }
}

void ClassGeneration::generateClassFile(ClassTableElement* cls) {
    std::string outDir = "bytecode";
    std::string className = cls->clsName;
    std::filesystem::create_directories(outDir);
    m_filename = outDir + "/" + className + ".class";
    m_out.open(m_filename, std::ios::binary | std::ios::trunc);

    if (!m_out.is_open()) {
        throw runtime_error("Cannot create file: " + m_filename);
    }

    m_buffer.clear();
    generate(cls);
    m_out.close();
    printf("Generated: %s\n", m_filename.c_str());
}

void ClassGeneration::generate(ClassTableElement* cls) {
    m_buffer.clear();

    int m_thisClassIndex = findClassConstantIndex(cls, cls->clsName);
    int m_superClassIndex = findClassConstantIndex(cls, "java/lang/Object");

    writeHeader(cls);
    writeU2(0x0021); // ACC_PUBLIC | ACC_SUPER

    // THIS / SUPER
    writeU2(m_thisClassIndex);
    writeU2(m_superClassIndex);

    writeU2(0); // interfaces_count
    writeU2(0); // fields_count

    std::vector<uint8_t> methodsBytes;
    uint16_t methodsCount = 0;

    // ===== 7.1 <init> =====
    if (cls->methods->methods.count("<init>_()V")) {
        auto initMethod = generateInitMethod(cls, cls->methods->methods["<init>_()V"]);
        methodsBytes.insert(methodsBytes.end(), initMethod.begin(), initMethod.end());
        methodsCount++;
    }

    // ===== 7.2 main =====
    if (cls->methods->methods.count("main_()V")) {
        auto mainMethod = generateMainMethod(cls, cls->methods->methods["main_()V"]);
        methodsBytes.insert(methodsBytes.end(), mainMethod.begin(), mainMethod.end());
        methodsCount++;
    }

    // ===== 7.3 обычные методы (кроме main) =====
    for (auto &pair : cls->methods->methods) {
        if (!pair.second) continue;
        if (pair.first == "<init>_()V" || pair.first == "main_()V") continue;

        auto m = generateMethod(cls, pair.second);
        methodsBytes.insert(methodsBytes.end(), m.begin(), m.end());
        methodsCount++;
    }

    // ===== 7.4 записываем count методов и сами методы =====
    writeU2(methodsCount);
    m_buffer.insert(m_buffer.end(), methodsBytes.begin(), methodsBytes.end());

    /* ========= 8. CLASS ATTRIBUTES ========= */
    writeU2(0); // attributes_count

    /* ========= 9. WRITE FILE ========= */
    writeToFile();
}


void ClassGeneration::writeHeader(ClassTableElement* cls) {
    writeU4(0xCAFEBABE);    // Magic number
    writeU2(0x0000);        // Minor version
    writeU2(0x0041);        // Major version (Java 21)

    // constantPool + size
    std::cout << "Constant table size: " + to_string(cls->constants->items.size()) << std::endl;
    uint16_t cpCount = static_cast<uint16_t>(cls->constants->items.size() + 1);
    writeU2(cpCount);
    std::vector<uint8_t> cpBytes = BytecodeGenerator::generateBytesForConstantTable(cls->constants);
    m_buffer.insert(m_buffer.end(), cpBytes.begin(), cpBytes.end());
}

void ClassGeneration::writeClassInfo(ClassTableElement* cls) {
    // this_class — индекс класса в constant pool
    int thisClassIndex = findClassConstantIndex(cls, cls->clsName);
    writeU2(thisClassIndex);

    // super_class = java/lang/Object (индекс 2 обычно)
    int superClassIndex = findClassConstantIndex(cls, "java/lang/Object");
    writeU2(superClassIndex);

    writeU2(0); // interfaces_count = 0
}

void ClassGeneration::writeFields(ClassTableElement* cls) {
    writeU2(0);
}

void ClassGeneration::writeToFile() {
    m_out.write(reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size());
    m_buffer.clear();
}

std::vector<uint8_t> ClassGeneration::generateMainMethod(ClassTableElement* elem, MethodTableElement* kotlinMain) {
    std::vector<uint8_t> res;

    writeU2Local(res, 0x0009);

    int mainNameUTF8 = elem->constants->findConstantUTF8("main");
    writeU2Local(res, mainNameUTF8);

    int jvmMainDesc = elem->constants->findOrAddConstant(UTF8, "([Ljava/lang/String;)V");

    writeU2Local(res, jvmMainDesc);

    writeU2Local(res, 1);

    int codeNameIdx = elem->constants->findConstantUTF8("Code");
    writeU2Local(res, codeNameIdx);

    std::vector<uint8_t> codeBytes;

    int nameAndTypeIdx = elem->constants->findConstantNameAndType(
        kotlinMain->methodName, kotlinMain->descriptor);
    if (nameAndTypeIdx != -1) {
        int selfRef = elem->constants->findConstantMethodRef(
            elem->name, nameAndTypeIdx);
        if (selfRef != -1) {
            // auto invoke = BytecodeGenerator::invokestatic(selfRef);
            // codeBytes.insert(codeBytes.end(), invoke.begin(), invoke.end());
        }
    }

    codeBytes.push_back(0xB1);
    uint32_t attrLength = 12 + codeBytes.size();
    writeU4Local(res, attrLength);
    writeU2Local(res, 1);
    writeU2Local(res, 1);
    writeU4Local(res, codeBytes.size());
    BytecodeGenerator::appendToByteArray(&res, codeBytes.data(), codeBytes.size());
    writeU2Local(res, 0);
    writeU2Local(res, 0);

    return res;
}


std::vector<uint8_t> ClassGeneration::generateMethod(ClassTableElement* elem, MethodTableElement *method) {
    std::vector<uint8_t> res;

    uint16_t flags = getMethodAccessFlags(method);
    BytecodeGenerator::appendToByteArray(&res, BytecodeGenerator::intToByteVector(flags, 2).data(), 2);
    BytecodeGenerator::appendToByteArray(&res, BytecodeGenerator::intToByteVector(method->methodName, 2).data(), 2);
    BytecodeGenerator::appendToByteArray(&res, BytecodeGenerator::intToByteVector(method->descriptor, 2).data(), 2);
    BytecodeGenerator::appendToByteArray(&res, BytecodeGenerator::intToByteVector(1, 2).data(), 2); // attributes_count = 1

    int codeNameIdx = elem->constants->findOrAddConstant(UTF8, "Code");
    writeU2Local(res, codeNameIdx);

    std::vector<uint8_t> code;

    // Обычные методы
    auto codeAttr = KotlinCodeGenerator::generate(elem, method);
    code.insert(code.end(), codeAttr.begin(), codeAttr.end());

    BytecodeGenerator::appendToByteArray(&res, code.data(), code.size());   // добавляем код в атрибуты

    return res;
}

void ClassGeneration::writeU2(uint16_t value) {
    m_buffer.push_back(value >> 8);
    m_buffer.push_back(value & 0xFF);
}

void ClassGeneration::writeU4(uint32_t value) {
    m_buffer.push_back((value >> 24) & 0xFF);
    m_buffer.push_back((value >> 16) & 0xFF);
    m_buffer.push_back((value >> 8) & 0xFF);
    m_buffer.push_back(value & 0xFF);
}

int ClassGeneration::findClassConstantIndex(ClassTableElement* cls, const std::string& className) {
    int utf8Index = cls->constants->findOrAddConstant(UTF8, className);
    return cls->constants->findOrAddConstant(Class, "", 0, 0, utf8Index);
}

void ClassGeneration::writeU2Local(std::vector<uint8_t>& buffer, uint16_t value) {
    buffer.push_back(value >> 8);
    buffer.push_back(value & 0xFF);
}

void ClassGeneration::writeU4Local(std::vector<uint8_t>& buffer, uint32_t value) {
    buffer.push_back((value >> 24) & 0xFF);  // байт 3 (старший)
    buffer.push_back((value >> 16) & 0xFF);  // байт 2
    buffer.push_back((value >> 8) & 0xFF);   // байт 1
    buffer.push_back(value & 0xFF);          // байт 0 (младший)
}

std::vector<uint8_t> ClassGeneration::generateInitMethod(ClassTableElement* cls, MethodTableElement* method) {
    std::vector<uint8_t> res;

    writeU2Local(res, 0x0001);            // access_flags = public
    writeU2Local(res, method->methodName);      // name_index
    writeU2Local(res, method->descriptor);      // descriptor_index — ()V
    writeU2Local(res, 1);                 // attributes_count = 1 (Code)

    // Code attribute
    int codeNameIdx = cls->constants->findOrAddConstant(UTF8, "Code");
    writeU2Local(res, codeNameIdx);

    // Код конструктора: aload_0; invokespecial java/lang/Object.<init>; return
    std::vector<uint8_t> codeBytes;
    codeBytes.push_back(0x2A);               // aload_0
    codeBytes.push_back(0xB7);

    int objectClassIdx = cls->constants->findOrAddConstant(UTF8, "java/lang/Object");
    int objectClassRefIdx = cls->constants->findOrAddConstant(Class, "", 0, 0, objectClassIdx);
    int initNameIdx = cls->constants->findOrAddConstant(UTF8, "<init>");
    int initDescIdx = cls->constants->findOrAddConstant(UTF8, "()V");
    int nameAndTypeIdx = cls->constants->findOrAddConstant(NameAndType, "", 0, 0, initNameIdx, initDescIdx);
    int initMethodRefIndex = cls->constants->findOrAddConstant(MethodRef, "", 0, 0, objectClassRefIdx, nameAndTypeIdx);
    writeU2Local(codeBytes, initMethodRefIndex); // invokespecial Object.<init>

    codeBytes.push_back(0xB1);               // return

    // attribute_length = max_stack + max_locals + code_length_field + code + exception_table + attributes_count
    uint32_t attrLength = 2 + 2 + 4 + codeBytes.size() + 2 + 2;
    writeU4Local(res, attrLength);

    writeU2Local(res, 1);  // max_stack
    writeU2Local(res, 1);  // max_locals
    writeU4Local(res, codeBytes.size());    // code_length

    BytecodeGenerator::appendToByteArray(&res, codeBytes.data(), codeBytes.size()); // сам код
    writeU2Local(res, 0);   // exception_table_length
    writeU2Local(res, 0);   // attributes_count

    return res;
}
