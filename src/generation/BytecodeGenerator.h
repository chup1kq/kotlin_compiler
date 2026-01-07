#ifndef KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H
#define KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H
#include <vector>
#include <cstdint>
#include <stdexcept>

enum class IfCommandType { EQ, NE, LT, LE, GT, GE };

class BytecodeGenerator {

private:
    std::vector<uint8_t> m_buffer;

    void appendArray(const uint8_t* array, size_t size);
    std::vector<uint8_t> intToByteVector(int num, int arraySize);

public:
    static constexpr uint8_t magicConstant[4] = {0xCA, 0xFE, 0xBA, 0xBE};
    static constexpr uint8_t smallVersion[2] = {0x00, 0x00};
    static constexpr uint8_t bigVersion[2] = {0x00, 0x41};  // Java 21

    // Константы
    std::vector<uint8_t> iconstBipushSipush(int num);

    // Локальные переменные
    std::vector<uint8_t> iload(int num);
    std::vector<uint8_t> aload(int num);
    std::vector<uint8_t> istore(int num);
    std::vector<uint8_t> astore(int num);

    // Стек
    std::vector<uint8_t> ldc(int num);
    std::vector<uint8_t> pop();
    std::vector<uint8_t> dup();
    std::vector<uint8_t> dup2();

    // Управление потоком
    std::vector<uint8_t> go_to(int offset);
    std::vector<uint8_t> if_icmp(IfCommandType type, int offset);
    std::vector<uint8_t> if_(IfCommandType type, int offset);

    // Методы
    std::vector<uint8_t> invokespecial(int constant);
    std::vector<uint8_t> invokevirtual(int constant);
    std::vector<uint8_t> invokestatic(int constant);

    // Возврат
    std::vector<uint8_t> ireturn();
    std::vector<uint8_t> areturn();
    std::vector<uint8_t> _return();

    // Объекты
    std::vector<uint8_t> _new(int constant);
    std::vector<uint8_t> getfield(int constant);
    std::vector<uint8_t> putfield(int constant);

    // Массивы
    std::vector<uint8_t> anewarray(int constant);
    std::vector<uint8_t> arraylength();
    std::vector<uint8_t> aastore();
    std::vector<uint8_t> aaload();

    // Арифметика
    std::vector<uint8_t> iadd();

    // Получить готовый байткод
    const std::vector<uint8_t>& getBytecode() const { return m_buffer; }
    void clear() { m_buffer.clear(); }

};


#endif //KOTLIN_COMPILER_CLASSGENERATIONHANDLER_H