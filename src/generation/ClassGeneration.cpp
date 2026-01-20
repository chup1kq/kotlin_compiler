#include "ClassGeneration.h"

#include <filesystem>
#include <iostream>

#include "BytecodeGenerator.h"
#include "KotlinCodeGenerator.h"

ClassGeneration::ClassGeneration(ClassTableElement* cls)
    : m_class(cls),
      m_constants(cls->constants),
      m_accessFlags(0x0021),
      m_superClassIndex(0),
      m_thisClassIndex(0) {
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


void ClassGeneration::generateClassFile(const std::string& className) {
    std::string outDir = "bytecode";
    std::filesystem::create_directories(outDir);
    m_filename = outDir + "/" + className + ".class";
    m_out.open(m_filename, std::ios::binary | std::ios::trunc);

    if (!m_out.is_open()) {
        throw runtime_error("Cannot create file: " + m_filename);
    }

    m_buffer.clear();
    generate();
    m_out.close();
    printf("Generated: %s\n", m_filename.c_str());
}

void ClassGeneration::generate() {
    m_buffer.clear();

    m_thisClassIndex = findClassConstantIndex(m_class->clsName);
    m_superClassIndex = findClassConstantIndex("java/lang/Object");

    // Заголовок
    writeHeader();

    // AccessFlags
    writeU2(0x0021); // ACC_PUBLIC | ACC_SUPER

    // THIS / SUPER
    writeU2(m_thisClassIndex);    // index класса MainKt
    writeU2(m_superClassIndex);   // index java/lang/Object

    // interfaces count
    writeU2(0); // interfaces_count

    // fields count
    writeU2(0); // fields_count

    // methods
    std::vector<uint8_t> methodsBytes;
    uint16_t methodsCount = 0;

    // ===== 7.1 <init> =====
    if (m_class->methods->methods.count("<init>_()V")) {
        auto initMethod = generateInitMethod(m_class->methods->methods["<init>_()V"]);
        methodsBytes.insert(methodsBytes.end(), initMethod.begin(), initMethod.end());
        methodsCount++;
    }

    // ===== 7.2 main =====
    if (m_class->methods->methods.count("main_()V")) {
        auto mainMethod = generateMainMethod(m_class, m_class->methods->methods["main_()V"]);
        methodsBytes.insert(methodsBytes.end(), mainMethod.begin(), mainMethod.end());
        methodsCount++;
    }

    // ===== 7.3 обычные методы (кроме main) =====
    for (auto &pair : m_class->methods->methods) {
        if (!pair.second) continue;
        if (pair.first == "<init>_()V" || pair.first == "main_()V") continue;

        auto m = generateMethod(m_class, pair.second);
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


void ClassGeneration::writeHeader() {
    writeU4(0xCAFEBABE);    // Magic number
    writeU2(0x0000);        // Minor version
    writeU2(0x0041);        // Major version (Java 21)

    // constantPool + size
    std::cout << "Constant table size: " + to_string(m_class->constants->items.size()) << std::endl;
    uint16_t cpCount = static_cast<uint16_t>(m_constants->items.size() + 1);
    writeU2(cpCount);
    std::vector<uint8_t> cpBytes = BytecodeGenerator::generateBytesForConstantTable(m_constants);
    m_buffer.insert(m_buffer.end(), cpBytes.begin(), cpBytes.end());
}

void ClassGeneration::writeClassInfo() {
    // this_class — индекс класса в constant pool
    m_thisClassIndex = findClassConstantIndex(m_class->clsName);
    writeU2(m_thisClassIndex);

    // super_class = java/lang/Object (индекс 2 обычно)
    m_superClassIndex = findClassConstantIndex("java/lang/Object");
    writeU2(m_superClassIndex);

    // interfaces_count = 0
    writeU2(0);
}

void ClassGeneration::writeFields() {
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

    // kotlinMain->descriptor = jvmMainDesc;
    // kotlinMain->strDesc = "([Ljava/lang/String;)V";

    writeU2Local(res, jvmMainDesc);

    writeU2Local(res, 1);

    int codeNameIdx = elem->constants->findConstantUTF8("Code");
    writeU2Local(res, codeNameIdx);

    std::vector<uint8_t> codeBytes;

    int nameAndTypeIdx = elem->constants->findConstantNameAndType(
        kotlinMain->methodName, kotlinMain->descriptor);
    if (nameAndTypeIdx != -1) {
        int selfRef = elem->constants->findConstantMethodRef(
            m_thisClassIndex, nameAndTypeIdx);
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

    int codeNameIdx = m_constants->findOrAddConstant(UTF8, "Code");
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

int ClassGeneration::findClassConstantIndex(const std::string& className) {
    int utf8Index = m_constants->findOrAddConstant(UTF8, className); // Ищем Utf8 constant
    return m_constants->findOrAddConstant(Class, "", 0, 0, utf8Index);  // Ищем/добавляем Class constant
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

std::vector<uint8_t> ClassGeneration::generateInitMethod(MethodTableElement* method) {
    std::vector<uint8_t> res;

    writeU2Local(res, 0x0001);            // access_flags = public
    writeU2Local(res, method->methodName);      // name_index
    writeU2Local(res, method->descriptor);      // descriptor_index — ()V
    writeU2Local(res, 1);                 // attributes_count = 1 (Code)

    // Code attribute
    int codeNameIdx = m_constants->findOrAddConstant(UTF8, "Code");
    writeU2Local(res, codeNameIdx);

    // Код конструктора: aload_0; invokespecial java/lang/Object.<init>; return
    std::vector<uint8_t> codeBytes;
    codeBytes.push_back(0x2A);               // aload_0
    codeBytes.push_back(0xB7);

    int objectClassIdx = m_constants->findOrAddConstant(UTF8, "java/lang/Object");
    int objectClassRefIdx = m_constants->findOrAddConstant(Class, "", 0, 0, objectClassIdx);
    int initNameIdx = m_constants->findOrAddConstant(UTF8, "<init>");
    int initDescIdx = m_constants->findOrAddConstant(UTF8, "()V");
    int nameAndTypeIdx = m_constants->findOrAddConstant(NameAndType, "", 0, 0, initNameIdx, initDescIdx);
    int initMethodRefIndex = m_constants->findOrAddConstant(MethodRef, "", 0, 0, objectClassRefIdx, nameAndTypeIdx);
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
