#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "value.hpp"

class Chunk {
  public:
    Chunk();
    ~Chunk();
    void write(uint8_t byte, int line);
    void disassemble(const std::string & name) const;
    size_t addConstant(const Value & value);

  private:
    size_t disassembleInstruction(size_t offset) const;
    size_t simpleInstruction(const std::string & name, size_t offset) const;
    size_t constantInstruction(const std::string & name, size_t offset) const;
    std::vector<uint8_t> m_code;
    std::vector<int> m_lines;
    std::vector<Value> m_constants;
};
