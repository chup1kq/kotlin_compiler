#include "BytecodeGenerator.h"

void BytecodeGenerator::appendArray(const uint8_t *array, size_t size) {

}

std::vector<uint8_t> BytecodeGenerator::intToByteVector(int num, int arraySize) {

}

std::vector<uint8_t> BytecodeGenerator::iconstBipushSipush(int num) {

}

std::vector<uint8_t> BytecodeGenerator::iload(int num) {

}

std::vector<uint8_t> BytecodeGenerator::aload(int num) {

}

std::vector<uint8_t> BytecodeGenerator::istore(int num) {

}

std::vector<uint8_t> BytecodeGenerator::astore(int num) {

}

std::vector<uint8_t> BytecodeGenerator::ldc(int num) {

}

std::vector<uint8_t> BytecodeGenerator::pop() {

}

std::vector<uint8_t> BytecodeGenerator::dup() {

}

std::vector<uint8_t> BytecodeGenerator::dup2() {

}

std::vector<uint8_t> BytecodeGenerator::go_to(int offset) {

}

std::vector<uint8_t> BytecodeGenerator::if_icmp(IfCommandType type, int offset) {

}

std::vector<uint8_t> BytecodeGenerator::if_(IfCommandType type, int offset) {

}

std::vector<uint8_t> BytecodeGenerator::invokespecial(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::invokevirtual(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::invokestatic(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::ireturn() {

}

std::vector<uint8_t> BytecodeGenerator::areturn() {

}

std::vector<uint8_t> BytecodeGenerator::_return() {

}

std::vector<uint8_t> BytecodeGenerator::_new(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::getfield(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::putfield(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::anewarray(int constant) {

}

std::vector<uint8_t> BytecodeGenerator::arraylength() {

}

std::vector<uint8_t> BytecodeGenerator::aastore() {

}

std::vector<uint8_t> BytecodeGenerator::aaload() {

}

std::vector<uint8_t> BytecodeGenerator::iadd() {

}