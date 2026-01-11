#ifndef KOTLIN_COMPILER_CLASSGENERATION_H
#define KOTLIN_COMPILER_CLASSGENERATION_H
#include <cstdint>
#include <fstream>

#include "../semantic/tables/class/ClassTableElement.h"

using namespace std;

class ClassGeneration {
private:
    // Основные данные
    ClassTableElement* m_class;             // Класс из ClassTable
    string m_filename;                      // "MyProgram.class"

    // Файловые структуры
    ofstream m_out;                  // Бинарный поток
    vector<uint8_t> m_buffer;        // Финальный .class

    // JVM секции
    ConstantTable* m_constants;             // Пул констант
    vector<uint8_t> m_fields;               // Поля (пока пустые)
    vector<uint8_t> m_methods;              // Методы класса

    // JVM метаданные
    uint16_t m_thisClassIndex;            // #15 this_class
    uint16_t m_superClassIndex;           // #7 java/lang/Object
    uint16_t m_accessFlags;               // 0x0021 ACC_PUBLIC

    void writeU2(uint16_t value);
    void writeU4(uint32_t value);
    void generate();

    void writeU2Local(std::vector<uint8_t>& buffer, uint16_t value);
    void writeU4Local(std::vector<uint8_t>& buffer, uint32_t value);

    size_t m_headerPos;

public:
    ClassGeneration(ClassTableElement* cls);

    void generateClassFile(const std::string& className);

    // Секции .class файла
    void writeHeader();           // Magic + version
    void writeConstantPool();     // Пул констант
    void writeClassInfo();        // this_class, super_class
    void writeFields();           // Поля класса
    void writeMethods();          // main() + пользовательские
    void writeToFile();           // Запись

    std::vector<uint8_t> generateMainMethod(ClassTableElement* elem, MethodTableElement* main);
    std::vector<uint8_t> generateMethod(MethodTableElement* method);

    int findClassConstantIndex(const std::string& className);
};

#endif //KOTLIN_COMPILER_CLASSGENERATION_H