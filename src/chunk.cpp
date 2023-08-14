#include "chunk.hpp"

#include <format>
#include <iomanip>
#include <iostream>

#include "opcode.hpp"

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
    std::cout << std::format("{:04} ", offset);

    if (offset > 0 && this->m_lines[offset] == this->m_lines[offset - 1]) {
        std::cout << "   | ";
    } else {
        std::cout << std::format(" {:<4}", m_lines[offset]);
    }

    uint8_t instruction = this->m_code[offset];
    switch (instruction) {
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVIDE", offset);
    case OP_EQUAL:
        return simpleInstruction("OP_EQUAL", offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
    case OP_GREATER:
        return simpleInstruction("OP_GREATER", offset);
    case OP_GREATER_EQUAL:
        return simpleInstruction("OP_GREATER_EQUAL", offset);
    case OP_LESS:
        return simpleInstruction("OP_LESS", offset);
    case OP_LESS_EQUAL:
        return simpleInstruction("OP_LESS_EQUAL", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_NOT_EQUAL:
        return simpleInstruction("OP_NOT_EQUAL", offset);
    case OP_NULL:
        return simpleInstruction("OP_NULL", offset);
    case OP_NOT:
        return simpleInstruction("OP_NOT", offset);
    case OP_PRINT:
        return simpleInstruction("OP_PRINT", offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_TRUE:
        return simpleInstruction("OP_TRUE", offset);
    default:
        std::cout << std::format("Unknown opcode {}", instruction) << std::endl;
        return offset + 1;
    }
}

[[nodiscard]] size_t Chunk::simpleInstruction(std::string const & name, size_t offset) const {
    std::cout << std::setfill(' ') << std::setw(16) << name << std::endl;
    return offset + 1;
}

[[nodiscard]] size_t Chunk::addConstant(Value const & value) {
    this->m_constants.push_back(value);
    return this->m_constants.size() - 1;
}

[[nodiscard]] size_t Chunk::constantInstruction(std::string const & name, size_t offset) const {
    uint8_t constant = this->m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16} '{}'", name, unsigned(constant), m_constants[constant].asString())
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