#ifndef KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H
#define KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "../semantic/tables/class/ClassTableElement.h"
#include "../semantic/tables/class/ClassTable.h"

enum class IfCommandType { EQ, NE, LT, LE, GT, GE };

class BytecodeGenerator {

private:
    std::vector<uint8_t> m_buffer;

    void appendArray(const uint8_t* array, size_t size);

public:
    static constexpr uint8_t magicConstant[4] = {0xCA, 0xFE, 0xBA, 0xBE};
    static constexpr uint8_t smallVersion[2] = {0x00, 0x00};
    static constexpr uint8_t bigVersion[2] = {0x00, 0x41};  // Java 21

    static void appendToByteArray(std::vector<uint8_t>* data, const uint8_t* array, size_t size);
    static std::vector<uint8_t> intToByteVector(int num, int arraySize);

    // Константы
    static std::vector<uint8_t> iconstBipushSipush(int num);

    // Локальные переменные
    static std::vector<uint8_t> iload(int num);
    static std::vector<uint8_t> aload(int num);
    static std::vector<uint8_t> istore(int num);
    static std::vector<uint8_t> astore(int num);

    // Стек
    static std::vector<uint8_t> ldc(int num);
    static std::vector<uint8_t> pop();
    static std::vector<uint8_t> dup();
    static std::vector<uint8_t> dup2();

    // Управление потоком
    static std::vector<uint8_t> go_to(int offset);
    static std::vector<uint8_t> if_icmp(IfCommandType type, int offset);
    static std::vector<uint8_t> if_(IfCommandType type, int offset);

    // Методы
    static std::vector<uint8_t> invokespecial(int constant);
    static std::vector<uint8_t> invokevirtual(int constant);
    static std::vector<uint8_t> invokestatic(int constant);

    // Возврат
    static std::vector<uint8_t> ireturn();
    static std::vector<uint8_t> areturn();
    static std::vector<uint8_t> _return();

    // Объекты
    static std::vector<uint8_t> _new(int constant);
    static std::vector<uint8_t> getfield(int constant);
    static std::vector<uint8_t> putfield(int constant);

    // Массивы
    static std::vector<uint8_t> anewarray(int constant);
    static std::vector<uint8_t> arraylength();
    static std::vector<uint8_t> aastore();
    static std::vector<uint8_t> aaload();

    // Арифметика
    static std::vector<uint8_t> iadd();

    // Получить готовый байткод
    const std::vector<uint8_t>& getBytecode() const { return m_buffer; }
    void clear() { m_buffer.clear(); }

    static std::vector<uint8_t> generateBytesForConstantTable(ConstantTable* table);
    static std::vector<uint8_t> generateBytesForConstantTableItem(ConstantTableElement* elem);

};


#endif //KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H