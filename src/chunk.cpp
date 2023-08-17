#include "chunk.hpp"

#include <format>
#include <iomanip>
#include <iostream>

Chunk::Chunk() {
}

Chunk::~Chunk() {
    m_code.clear();
}

void Chunk::write(uint8_t byte, int line) {
    m_code.push_back(byte);
    m_lines.push_back(line);
}

void Chunk::disassemble(std::string const & name) const {
    std::cout << std::format("== {} ==", name) << std::endl;

    for (size_t offset = 0; offset < m_code.size();) {
        offset = disassembleInstruction(offset);
    }
}

[[nodiscard]] size_t Chunk::disassembleInstruction(size_t offset) const {
    std::cout << std::format("{:#06X} ", offset);

    if (offset > 0 && this->m_lines[offset] == this->m_lines[offset - 1]) {
        std::cout << "   | ";
    } else {
        std::cout << std::format(" {:<4}", m_lines[offset]);
    }

    uint8_t instruction = this->m_code[offset];
    switch (instruction) {
    case OP_ADD:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_CONSTANT:
        return constantInstruction(static_cast<Opcode>(instruction), offset);
    case OP_DIVIDE:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_EQUAL:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_FALSE:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_GREATER:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_GREATER_EQUAL:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_LESS:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_LESS_EQUAL:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_MULTIPLY:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_NEGATE:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_NOT_EQUAL:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_NULL:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_NOT:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_PRINT:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_RETURN:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_SUBTRACT:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    case OP_TRUE:
        return simpleInstruction(static_cast<Opcode>(instruction), offset);
    default:
        std::cout << std::format("Unknown opcode {}", instruction) << std::endl;
        return offset + 1;
    }
}

[[nodiscard]] size_t Chunk::simpleInstruction(Opcode opcode, size_t offset) const {
    std::cout << std::format("{:>16}", opcode_as_string(opcode)) << std::endl;
    return offset + 1;
}

[[nodiscard]] size_t Chunk::addConstant(Value const & value) {
    this->m_constants.push_back(value);
    return this->m_constants.size() - 1;
}

[[nodiscard]] size_t Chunk::constantInstruction(Opcode opcode, size_t offset) const {
    uint8_t constant = this->m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16} '{}'", opcode_as_string(opcode), unsigned(constant), m_constants[constant])
              << std::endl;
    return offset + 2;
}

[[nodiscard]] uint8_t Chunk::getByte(size_t offset) const {
    return this->m_code[offset];
}

[[nodiscard]] size_t Chunk::getLine(size_t offset) const {
    return this->m_lines[offset];
}
[[nodiscard]] Value const & Chunk::getConstant(size_t offset) const {
    return this->m_constants[offset];
}
[[nodiscard]] size_t Chunk::getSize() const {
    return this->m_code.size();
}