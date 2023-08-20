#include "chunk.hpp"

#include <format>
#include <iomanip>
#include <iostream>

#include "../types/value_formatter.hpp"
#include "opcode_formatter.hpp"

using namespace cppLox::ByteCode;

Chunk::Chunk() {
}

Chunk::~Chunk() {
    m_code.clear();
}

auto Chunk::write(uint8_t byte, int line) -> void {
    m_code.push_back(byte);
    m_lines.push_back(line);
}

auto Chunk::write(Opcode byte, int line) -> void {
    m_code.push_back(static_cast<uint8_t>(byte));
    m_lines.push_back(line);
}

auto Chunk::disassemble(std::string_view const & name) const -> void {
    std::cout << std::format("== {} ==", name) << std::endl;

    for (size_t offset = 0; offset < m_code.size();) {
        offset = disassembleInstruction(offset);
    }
}

[[nodiscard]] auto Chunk::disassembleInstruction(size_t offset) const -> size_t {
    std::cout << std::format("{:#06X} ", offset);

    if (offset > 0 && m_lines[offset] == m_lines[offset - 1]) {
        std::cout << "   | ";
    } else {
        std::cout << std::format(" {:<4}", m_lines[offset]);
    }

    uint8_t instruction = m_code[offset];
    switch (static_cast<Opcode>(instruction)) {
    case Opcode::ADD:
        return simpleInstruction(instruction, offset);
    case Opcode::CONSTANT:
        return constantInstruction(instruction, offset);
    case Opcode::DIVIDE:
        return simpleInstruction(instruction, offset);
    case Opcode::EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::FALSE:
        return simpleInstruction(instruction, offset);
    case Opcode::GREATER:
        return simpleInstruction(instruction, offset);
    case Opcode::GREATER_EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::LESS:
        return simpleInstruction(instruction, offset);
    case Opcode::LESS_EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::MULTIPLY:
        return simpleInstruction(instruction, offset);
    case Opcode::NEGATE:
        return simpleInstruction(instruction, offset);
    case Opcode::NOT_EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::NULL_:
        return simpleInstruction(instruction, offset);
    case Opcode::NOT:
        return simpleInstruction(instruction, offset);
    case Opcode::PRINT:
        return simpleInstruction(instruction, offset);
    case Opcode::RETURN:
        return simpleInstruction(instruction, offset);
    case Opcode::SUBTRACT:
        return simpleInstruction(instruction, offset);
    case Opcode::TRUE:
        return simpleInstruction(instruction, offset);
    default:
        std::cout << std::format("Unknown opcode {}", instruction) << std::endl;
        return offset + 1;
    }
}

[[nodiscard]] auto Chunk::simpleInstruction(uint8_t opcode, size_t offset) const -> size_t {
    std::cout << std::format("{:>16}", static_cast<Opcode>(opcode)) << std::endl;
    return offset + 1;
}

[[nodiscard]] auto Chunk::addConstant(cppLox::Types::Value const & value) -> size_t {
    m_constants.push_back(value);
    return m_constants.size() - 1;
}

[[nodiscard]] auto Chunk::constantInstruction(uint8_t opcode, size_t offset) const -> size_t {
    uint8_t constant = m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16} '{}'", static_cast<Opcode>(opcode), unsigned(constant),
                             m_constants[constant])
              << std::endl;
    return offset + 2;
}

[[nodiscard]] auto Chunk::getByte(size_t offset) const -> uint8_t {
    return m_code[offset];
}

[[nodiscard]] auto Chunk::getLine(size_t offset) const -> size_t {
    return m_lines[offset];
}
[[nodiscard]] auto Chunk::getConstant(size_t offset) const -> cppLox::Types::Value const & {
    return m_constants[offset];
}
[[nodiscard]] auto Chunk::getSize() const -> size_t {
    return m_code.size();
}