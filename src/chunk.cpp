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
        return simpleInstruction(instruction, offset);
    case OP_CONSTANT:
        return constantInstruction(instruction, offset);
    case OP_DIVIDE:
        return simpleInstruction(instruction, offset);
    case OP_EQUAL:
        return simpleInstruction(instruction, offset);
    case OP_FALSE:
        return simpleInstruction(instruction, offset);
    case OP_GREATER:
        return simpleInstruction(instruction, offset);
    case OP_GREATER_EQUAL:
        return simpleInstruction(instruction, offset);
    case OP_LESS:
        return simpleInstruction(instruction, offset);
    case OP_LESS_EQUAL:
        return simpleInstruction(instruction, offset);
    case OP_MULTIPLY:
        return simpleInstruction(instruction, offset);
    case OP_NEGATE:
        return simpleInstruction(instruction, offset);
    case OP_NOT_EQUAL:
        return simpleInstruction(instruction, offset);
    case OP_NULL:
        return simpleInstruction(instruction, offset);
    case OP_NOT:
        return simpleInstruction(instruction, offset);
    case OP_PRINT:
        return simpleInstruction(instruction, offset);
    case OP_RETURN:
        return simpleInstruction(instruction, offset);
    case OP_SUBTRACT:
        return simpleInstruction(instruction, offset);
    case OP_TRUE:
        return simpleInstruction(instruction, offset);
    default:
        std::cout << std::format("Unknown opcode {}", instruction) << std::endl;
        return offset + 1;
    }
}

[[nodiscard]] size_t Chunk::simpleInstruction(uint8_t opcode, size_t offset) const {
    std::cout << std::format("{:>16}", static_cast<Opcode>(opcode)) << std::endl;
    return offset + 1;
}

[[nodiscard]] size_t Chunk::addConstant(Value const & value) {
    this->m_constants.push_back(value);
    return this->m_constants.size() - 1;
}

[[nodiscard]] size_t Chunk::constantInstruction(uint8_t opcode, size_t offset) const {
    uint8_t constant = this->m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16} '{}'", static_cast<Opcode>(opcode), unsigned(constant),
                             m_constants[constant])
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