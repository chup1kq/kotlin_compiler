#include "ClassGeneration.h"

#include "BytecodeGenerator.h"

ClassGeneration::ClassGeneration(ClassTableElement* cls, const std::string& filename)
    : m_class(cls),
      m_constants(cls->constants),
      m_accessFlags(0x0021),
      m_superClassIndex(0),
      m_thisClassIndex(0) {
}

void ClassGeneration::generateClassFile(const std::string& className) {
    m_filename = "lexer-build/" + className + ".class";
    m_out.open(m_filename, ios::binary | ios::trunc);

    if (!m_out.is_open()) {
        throw runtime_error("Cannot create file: " + m_filename);
    }

    m_buffer.clear();
    generate();
    m_out.close();
    printf("Generated: %s\n", m_filename.c_str());
}

void ClassGeneration::generate() {
    m_headerPos = 8;        // запомни позицию constant_pool_count
    writeHeader();          // CAFEBABE + версии + count=0
    writeConstantPool();    // заполняет count и pool
    writeClassInfo();       // this_class, super_class
    writeFields();          // поля класса
    writeMethods();         // методы
    writeToFile();          // запись в файл
}

void ClassGeneration::writeHeader() {
    // 4 байта: Magic number (подпись Java)
    writeU4(0xCAFEBABE);

    // 2 байта: minor_version = 0
    writeU2(0x0000);

    // 2 байта: major_version = 65 (Java 21)
    writeU2(0x0041);

    // 2 байта: constant_pool_count
    writeU2(0);
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
    uint16_t count = 0;

    for (const auto& namePair : m_class->methods->methods) {
        count += namePair.second.size();
    }

    writeU2(count);

    for (const auto& namePair : m_class->methods->methods) {
        for (const auto& descPair : namePair.second) {
            MethodTableElement* method = descPair.second;

            auto bytes = generateMethod(method);
            m_buffer.insert(m_buffer.end(), bytes.begin(), bytes.end());
        }
    }
}

void ClassGeneration::writeToFile() {
    m_out.write(reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size());
    m_buffer.clear();
}

void ClassGeneration::generateMainMethod() {
    // пока пустой
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
