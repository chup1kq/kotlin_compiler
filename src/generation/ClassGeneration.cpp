#include "ClassGeneration.h"

ClassGeneration::ClassGeneration(ClassTableElement* cls, const std::string& filename)
    : m_class(cls),
      m_constants(cls->constants),
      m_accessFlags(0x0021),
      m_superClassIndex(0),
      m_thisClassIndex(0) {
    m_filename = "lexer-build/" + filename + ".class";
    m_out.open(m_filename, ios::binary | ios::trunc);

    if (!m_out.is_open()) {
        throw runtime_error("Cannot create file: " + m_filename);
    }
}

void ClassGeneration::generate() {
    writeHeader();
    writeConstantPool();
    writeClassInfo();
    writeFields();
    writeMethods();
    writeToFile();
    m_out.close();
    printf("Generated: %s\n", m_filename.c_str());
}

void ClassGeneration::writeHeader() {
    // 4 байта: Magic number (подпись Java)
    writeU4(0xCAFEBABE);

    // 2 байта: minor_version = 0
    writeU2(0x0000);

    // 2 байта: major_version = 65 (Java 21)
    writeU2(0x0041);

    // 2 байта: constant_pool_count (ЗАМЕНИМ после writeConstantPool())
    writeU2(0);
}

void ClassGeneration::writeConstantPool() {
}

void ClassGeneration::writeClassInfo() {
}

void ClassGeneration::writeFields() {
}

void ClassGeneration::writeMethods() {
}

void ClassGeneration::writeToFile() {
}

void ClassGeneration::generateMainMethod() {
}

std::vector<uint8_t> ClassGeneration::generateMethod(MethodTableElement *method) {
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
