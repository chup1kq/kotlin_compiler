#include "ClassGeneration.h"

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
    m_filename = "" + className + ".class";
    m_out.open(m_filename, ios::binary | ios::trunc);

    if (!m_out.is_open()) {
        throw runtime_error("Cannot create file: " + m_filename);
    }

    m_buffer.clear();
    generate();
    m_out.close();
    printf("Generated: %s\n", m_filename.c_str());
}

// void ClassGeneration::generate() {
//     m_headerPos = 8;        // запомни позицию constant_pool_count
//     writeHeader();          // CAFEBABE + версии + count=0
//     writeConstantPool();    // заполняет count и pool
//     writeClassInfo();       // this_class, super_class
//     writeFields();          // поля класса
//     writeMethods();         // методы
//     writeToFile();          // запись в файл
// }

void ClassGeneration::generate() {
    m_buffer.clear();

    /* ========= 1. MAGIC + VERSION ========= */
    writeU4(0xCAFEBABE);   // magic
    writeU2(0x0000);       // minor_version
    writeU2(0x0041);       // major_version (Java 21)

    /* ========= 2. CONSTANT POOL ========= */
    uint16_t cpCount = static_cast<uint16_t>(m_constants->items.size() + 1);
    writeU2(cpCount);

    std::vector<uint8_t> cpBytes =
        BytecodeGenerator::generateBytesForConstantTable(m_constants);

    m_buffer.insert(m_buffer.end(), cpBytes.begin(), cpBytes.end());

    /* ========= 3. ACCESS FLAGS ========= */
    writeU2(0x0021); // ACC_PUBLIC | ACC_SUPER

    /* ========= 4. THIS / SUPER ========= */
    writeU2(m_class->thisClass);    // index of Class "MainKt"
    writeU2(m_class->superClass);   // index of Class "java/lang/Object"

    /* ========= 5. INTERFACES ========= */
    writeU2(0); // interfaces_count

    /* ========= 6. FIELDS ========= */
    writeU2(0); // fields_count

    /* ========= 7. METHODS ========= */
    std::vector<uint8_t> methodsBytes;
    uint16_t methodsCount = 0;

    // обычные методы
    for (auto &pair : m_class->methods->methods) {
        if (!pair.second) continue;
        if (pair.first == "main") continue; // main отдельно

        auto m = generateMethod(m_class, pair.second);
        methodsBytes.insert(methodsBytes.end(), m.begin(), m.end());
        methodsCount++;
    }

    // main
    if (m_class->methods->methods.count("main")) {
        auto mainMethod =
            generateMainMethod(m_class, m_class->methods->methods["main"]);
        methodsBytes.insert(methodsBytes.end(),
                            mainMethod.begin(),
                            mainMethod.end());
        methodsCount++;
    }

    writeU2(methodsCount);
    m_buffer.insert(m_buffer.end(),
                    methodsBytes.begin(),
                    methodsBytes.end());

    /* ========= 8. CLASS ATTRIBUTES ========= */
    writeU2(0); // attributes_count

    /* ========= 9. WRITE FILE ========= */
    writeToFile();
}


void ClassGeneration::writeHeader() {
    // 4 байта: Magic number (подпись Java)
    writeU4(0xCAFEBABE);

    // 2 байта: minor_version = 0
    writeU2(0x0000);

    // 2 байта: major_version = 65 (Java 21)
    writeU2(0x0041);

    std::cout << "Constant table size: " + to_string(m_class->constants->items.size()) << std::endl;

    // 2 байта: constant_pool_count
    uint16_t poolCount = static_cast<uint16_t>(m_class->constants->items.size() + 1);
    writeU2(poolCount);
}

void ClassGeneration::writeConstantPool() {
    // Генерируем все байты constant pool
    std::vector<uint8_t> poolBytes = BytecodeGenerator::generateBytesForConstantTable(m_constants);

    // Записываем количество констант (+1 для индексации с 1)
    uint16_t count = poolBytes.size() > 0 ? m_constants->items.size() + 1 : 1;

    // Обновляем constant_pool_count в заголовке (на позиции 8-9)
    size_t headerPos = 8;
    m_buffer[headerPos] = (count >> 8) & 0xFF;
    m_buffer[headerPos + 1] = count & 0xFF;

    // Записываем сам constant pool
    m_buffer.insert(m_buffer.end(), poolBytes.begin(), poolBytes.end());
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

void ClassGeneration::writeMethods() {
    uint16_t count = static_cast<uint16_t>(m_class->methods->methods.size());

    writeU2(count);

    for (const auto& [fullMethodName, method] : m_class->methods->methods) {
        auto bytes = generateMethod(method);
        m_buffer.insert(m_buffer.end(), bytes.begin(), bytes.end());
    }
}

void ClassGeneration::writeToFile() {
    m_out.write(reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size());
    m_buffer.clear();
}

std::vector<uint8_t> ClassGeneration::generateMainMethod(ClassTableElement* elem, MethodTableElement* main) {
    std::vector<uint8_t> res;

    int d = main->descriptor;
    int n = main->methodName;
    int nam = elem->constants->findOrAddConstant(UTF8, "main");
    int desc = elem->constants->findOrAddConstant(UTF8, "([Ljava/lang/String;)V");
    int nat = elem->constants->findOrAddConstant(NameAndType, "",0,0,n,d);
    int mRef = elem->constants->findOrAddConstant(MethodRef, "",0,0,elem->thisClass,nat);


    uint8_t publicStaticFlag[2] = { 0x00, 0x09 }; //ACC_PUBLIC + ACC_STATIC
    BytecodeGenerator::appendToByteArray(&res, publicStaticFlag, 2);

    //Добавление имени метода
    std::vector<uint8_t> nameBytes = BytecodeGenerator::intToByteVector(nam, 2);
    BytecodeGenerator::appendToByteArray(&res, nameBytes.data(), nameBytes.size());

    // Добавление дескриптора метода
    std::vector<uint8_t> typeBytes = BytecodeGenerator::intToByteVector(desc, 2);
    BytecodeGenerator::appendToByteArray(&res, typeBytes.data(), typeBytes.size());

    //Добавление атрибутов TODO:Code
    std::vector<uint8_t> codeAttributeSizeBytes = BytecodeGenerator::intToByteVector(1, 2);
    BytecodeGenerator::appendToByteArray(&res, codeAttributeSizeBytes.data(), codeAttributeSizeBytes.size());


    int cd = elem->constants->findOrAddConstant(UTF8, "Code");
    codeAttributeSizeBytes = BytecodeGenerator::intToByteVector(cd, 2);
    BytecodeGenerator::appendToByteArray(&res, codeAttributeSizeBytes.data(), codeAttributeSizeBytes.size());

    std::vector<uint8_t> codeBytes;

    codeBytes = BytecodeGenerator::invokestatic(mRef);

    std::vector<uint8_t> ret = BytecodeGenerator::_return();

    BytecodeGenerator::appendToByteArray(&codeBytes, ret.data(), ret.size());

    //Добавление длины атрибута
    std::vector<uint8_t> lengthBytes = BytecodeGenerator::intToByteVector(12 + codeBytes.size(), 4);
    BytecodeGenerator::appendToByteArray(&res, lengthBytes.data(), lengthBytes.size());

    //Добавление размера стека операндов
    std::vector<uint8_t> stackSizeBytes = BytecodeGenerator::intToByteVector(1000, 2);
    BytecodeGenerator::appendToByteArray(&res, stackSizeBytes.data(), stackSizeBytes.size());

    //Добавление количества локальных переменных
    int localsSize = 1;
    std::vector<uint8_t> localsSizeBytes = BytecodeGenerator::intToByteVector(localsSize, 2);
    BytecodeGenerator::appendToByteArray(&res, localsSizeBytes.data(), localsSizeBytes.size());


    //Добавление длины байт-кода TODO: сделать
    std::vector<uint8_t> codeSizeBytes = BytecodeGenerator::intToByteVector(codeBytes.size(), 4);
    BytecodeGenerator::appendToByteArray(&res, codeSizeBytes.data(), codeSizeBytes.size());

    //Добавление байт-кода
    BytecodeGenerator::appendToByteArray(&res, codeBytes.data(), codeBytes.size());

    //Добавление количества записей в таблице исключений
    std::vector<uint8_t> exceptionTableSizeBytes = BytecodeGenerator::intToByteVector(0, 2);
    BytecodeGenerator::appendToByteArray(&res, exceptionTableSizeBytes.data(), exceptionTableSizeBytes.size());

    //Добавление количества атрибутов
    std::vector<uint8_t> attributesCountBytes = BytecodeGenerator::intToByteVector(0, 2);
    BytecodeGenerator::appendToByteArray(&res, attributesCountBytes.data(), attributesCountBytes.size());


    return res;
}

std::vector<uint8_t> ClassGeneration::generateMethod(MethodTableElement *method) {
    std::vector<uint8_t> res;

    // access_flags (public static = 0x0009)
    writeU2Local(res, 0x0009);

    // name_index — метод уже знает свой индекс в constant pool!
    writeU2Local(res, method->methodName);

    // descriptor_index — тоже готовый индекс!
    writeU2Local(res, method->descriptor);

    // attributes_count = 1 (Code attribute)
    writeU2Local(res, 1);

    // Code атрибут (пока пустой)
    int codeNameIdx = m_constants->findOrAddConstant(UTF8, "Code");
    writeU2Local(res, codeNameIdx);
    writeU4Local(res, 12);  // attribute_length

    writeU2Local(res, 1);  // max_stack
    writeU2Local(res, 1);  // max_locals
    writeU4Local(res, 1);  // code_length
    res.push_back(0x00);   // nop
    writeU2Local(res, 0);  // exception_table_length = 0

    return res;
}

std::vector<uint8_t> ClassGeneration::generateMethod(ClassTableElement* elem, MethodTableElement *method) {
    std::vector<uint8_t> res;

    uint16_t flags = getMethodAccessFlags(method);
    BytecodeGenerator::appendToByteArray(
        &res,
        BytecodeGenerator::intToByteVector(flags, 2).data(),
        2
    );


    //Добавление имени метода
    std::vector<uint8_t> nameBytes = BytecodeGenerator::intToByteVector(method->methodName, 2);
    BytecodeGenerator::appendToByteArray(&res, nameBytes.data(), nameBytes.size());

    // Добавление дескриптора метода
    std::vector<uint8_t> typeBytes = BytecodeGenerator::intToByteVector(method->descriptor, 2);
    BytecodeGenerator::appendToByteArray(&res, typeBytes.data(), typeBytes.size());

    //Добавление атрибутов TODO:Code
    std::vector<uint8_t> codeAttributeSizeBytes = BytecodeGenerator::intToByteVector(1, 2);
    BytecodeGenerator::appendToByteArray(&res, codeAttributeSizeBytes.data(), codeAttributeSizeBytes.size());
    std::vector<uint8_t> codeAttributeBytes = KotlinCodeGenerator::generate(elem, method);
    BytecodeGenerator::appendToByteArray(&res, codeAttributeBytes.data(), codeAttributeBytes.size());

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
    // Ищем Utf8 constant
    int utf8Index = m_constants->findOrAddConstant(UTF8, className);

    // Ищем/добавляем Class constant
    return m_constants->findOrAddConstant(Class, "", 0, 0, utf8Index);
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
