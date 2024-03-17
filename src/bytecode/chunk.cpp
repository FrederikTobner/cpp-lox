/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file chunk.cpp
 * @brief This file contains the implementation of the Chunk class.
 */

#include "chunk.hpp"

#include <format>
#include <iomanip>
#include <iostream>

#include "../types/value_formatter.hpp"
#include "opcode_formatter.hpp"

using namespace cppLox::ByteCode;

auto Chunk::write(uint8_t byte, int line) -> void {
    m_code.push_back(byte);
    m_lines.push_back(line);
}

auto Chunk::write(Opcode byte, int line) -> void {
    m_code.push_back(static_cast<uint8_t>(byte));
    m_lines.push_back(line);
}

auto Chunk::writeAt(size_t offset, uint8_t byte) -> void {
    m_code[offset] = byte;
}

auto Chunk::disassemble(std::string_view const & name) const -> void {
    std::cout << std::format("== {} ==", name) << std::endl;

    for (size_t offset = 0; offset < m_code.size();) {
        offset = disassembleInstruction(offset);
    }
}

auto Chunk::disassembleInstruction(size_t offset) const -> size_t {
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
    case Opcode::DEFINE_GLOBAL:
        return constantInstruction(instruction, offset);
    case Opcode::DIVIDE:
        return simpleInstruction(instruction, offset);
    case Opcode::EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::FALSE:
        return simpleInstruction(instruction, offset);
    case Opcode::GET_GLOBAL:
        return constantInstruction(instruction, offset);
    case Opcode::GET_LOCAL:
        return byteInstruction(instruction, offset);
    case Opcode::GREATER:
        return simpleInstruction(instruction, offset);
    case Opcode::GREATER_EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::JUMP:
        return jumpInstruction(instruction, 1, offset);
    case Opcode::JUMP_IF_FALSE:
        return jumpInstruction(instruction, 1, offset);
    case Opcode::LESS:
        return simpleInstruction(instruction, offset);
    case Opcode::LESS_EQUAL:
        return simpleInstruction(instruction, offset);
    case Opcode::LOOP:
        return jumpInstruction(instruction, -1, offset);
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
    case Opcode::POP:
        return simpleInstruction(instruction, offset);
    case Opcode::PRINT:
        return simpleInstruction(instruction, offset);
    case Opcode::RETURN:
        return simpleInstruction(instruction, offset);
    case Opcode::SET_GLOBAL:
        return constantInstruction(instruction, offset);
    case Opcode::SET_LOCAL:
        return byteInstruction(instruction, offset);
    case Opcode::SUBTRACT:
        return simpleInstruction(instruction, offset);
    case Opcode::TRUE:
        return simpleInstruction(instruction, offset);
    default:
        std::cout << std::format("Unknown opcode {}", unsigned(instruction)) << std::endl;
        return offset + 1;
    }
}

auto Chunk::simpleInstruction(uint8_t opcode, size_t offset) const -> size_t {
    std::cout << std::format("{:>16}", static_cast<Opcode>(opcode)) << std::endl;
    return offset + 1;
}

auto Chunk::addConstant(cppLox::Types::Value const & value) -> size_t {
    m_constants.push_back(value);
    return m_constants.size() - 1;
}

auto Chunk::byteInstruction(uint8_t opcode, size_t offset) const -> size_t {
    uint8_t slot = m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16}", static_cast<Opcode>(opcode), unsigned(slot)) << std::endl;
    return offset + 2;
}

auto Chunk::constantInstruction(uint8_t opcode, size_t offset) const -> size_t {
    uint8_t constant = m_code[offset + 1];
    std::cout << std::format("{:>16}{:>16} '{}'", static_cast<Opcode>(opcode), unsigned(constant),
                             m_constants[constant])
              << std::endl;
    return offset + 2;
}

auto Chunk::getByte(size_t offset) const -> uint8_t {
    return m_code[offset];
}

auto Chunk::getLine(size_t offset) const -> size_t {
    return m_lines[offset];
}

auto Chunk::getConstant(size_t offset) -> cppLox::Types::Value & {
    return m_constants[offset];
}

auto Chunk::getSize() const -> size_t {
    return m_code.size();
}

[[nodiscard]] auto Chunk::code() -> std::vector<uint8_t> & {
    return m_code;
}

auto Chunk::jumpInstruction(uint8_t opcode, int sign, int offset) const -> size_t {
    uint16_t const jump = static_cast<uint16_t>(m_code[offset + 1] << 8 | m_code[offset + 2]);
    std::cout << std::format("{:>16}{:>16} -> {}", static_cast<Opcode>(opcode), unsigned(jump),
                             offset + 3 + sign * jump)
              << std::endl;
    return offset + 3;
}
