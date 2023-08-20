#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "../types/value.hpp"
#include "opcode.hpp"


class Chunk {
  public:
    Chunk();
    ~Chunk();
    void write(uint8_t byte, int line);
    void write(Opcode byte, int line);
    void disassemble(std::string_view const & name) const;
    [[nodiscard]] size_t addConstant(Value const & value);
    [[nodiscard]] uint8_t getByte(size_t offset) const;
    [[nodiscard]] size_t getLine(size_t offset) const;
    [[nodiscard]] Value const & getConstant(size_t offset) const;
    [[nodiscard]] size_t getSize() const;
    [[nodiscard]] size_t disassembleInstruction(size_t offset) const;

  private:
    [[nodiscard]] size_t simpleInstruction(uint8_t opcode, size_t offset) const;
    [[nodiscard]] size_t constantInstruction(uint8_t opcode, size_t offset) const;
    std::vector<uint8_t> m_code;
    std::vector<int> m_lines;
    std::vector<Value> m_constants;
};
