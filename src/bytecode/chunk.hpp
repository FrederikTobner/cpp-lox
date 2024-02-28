#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "../types/value.hpp"
#include "opcode.hpp"

namespace cppLox::ByteCode {

/// @brief A chunk of bytecode.
class Chunk {
  public:
    /// @brief Constructs a new chunk.
    Chunk() = default;

    /// @brief Destructor of the chunk.
    ~Chunk() = default;

    /// @brief Writes a byte to the chunk.
    /// @param byte The byte to write.
    /// @param line The line number where the byte originates from the source code.
    auto write(uint8_t byte, int line) -> void;

    /// @brief Writes an opcode to the chunk.
    /// @param byte The opcode to write.
    /// @param line The line number where the opcode originates from the source code.
    auto write(Opcode byte, int line) -> void;

    /// @brief Writes a byte to the chunk.
    /// @param offset The offset to write the byte at.
    /// @param byte The byte to write.
    auto writeAt(size_t offset, uint8_t byte) -> void;

    /// @brief Disassembles the chunk.
    /// @param name The name of the chunk.
    auto disassemble(std::string_view const & name) const -> void;

    /// @brief Adds a constant to the chunk.
    /// @param value The value to add.
    /// @return The index of the added constant.
    [[nodiscard]] auto addConstant(cppLox::Types::Value const & value) -> size_t;

    /// @brief Gets the byte at the given index.
    /// @param index The index of the byte.
    /// @return The byte at the given index.
    [[nodiscard]] auto getByte(size_t index) const -> uint8_t;

    /// @brief Gets the line number of the given index.
    /// @param index The index of the opcode.
    /// @return The line number of the opcode at the given index.
    [[nodiscard]] auto getLine(size_t index) const -> size_t;

    /// @brief Gets the constant at the given index.
    /// @param index The index of the constant.
    /// @return The constant at the given index.
    [[nodiscard]] auto getConstant(size_t index) -> cppLox::Types::Value &;

    /// @brief Gets the size of the chunk.
    /// @return The size of the chunk.
    [[nodiscard]] auto getSize() const -> size_t;

    /// @brief Disassembles the instruction at the given offset.
    /// @param index The index of the instruction.
    /// @return The index of the next instruction.
    auto disassembleInstruction(size_t index) const -> size_t;

  private:
    /// @brief Disassembles a simple instruction.
    /// @param opcode The opcode of the instruction.
    /// @param index The index of the instruction.
    [[nodiscard]] auto simpleInstruction(uint8_t opcode, size_t index) const -> size_t;

    /// @brief Disassembles a constant instruction.
    /// @param opcode The opcode of the instruction.
    /// @param index The index of the instruction.
    [[nodiscard]] auto constantInstruction(uint8_t opcode, size_t index) const -> size_t;

    /// @brief Disassembles a byte instruction.
    /// @param opcode The opcode of the instruction.
    /// @param offset The offset of the instruction.
    /// @return The index of the next instruction.
    [[nodiscard]] auto byteInstruction(uint8_t opcode, size_t offset) const -> size_t;

    /// @brief Disassembles a jump instruction.
    /// @param opcode The opcode of the instruction.
    /// @param sign Whether the jump is forward or backward.
    /// @param offset The offset of the jump.
    /// @return The index of the next instruction.
    [[nodiscard]] auto jumpInstruction(uint8_t opcode, int sign, int offset) const -> size_t;

    /// @brief The bytecode stored in the chunk.
    std::vector<uint8_t> m_code;

    /// @brief The line numbers of the opcodes, stored in the chunk.
    std::vector<int> m_lines;

    /// @brief The value constants stored in the chunk.
    std::vector<cppLox::Types::Value> m_constants;
};
} // namespace cppLox::ByteCode