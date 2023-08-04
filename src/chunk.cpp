#include "chunk.hpp"

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

void Chunk::disassemble(const std::string & name) const {
    std::cout << "== " << name << " ==" << std::endl;

    for (size_t offset = 0; offset < m_code.size();) {
        offset = disassembleInstruction(offset);
    }
}

size_t Chunk::disassembleInstruction(size_t offset) const {
    std::cout << std::setfill('0') << std::setw(4) << offset << " ";

    if (offset > 0 && this->m_lines[offset] == this->m_lines[offset - 1]) {
        std::cout << "   | ";
    } else {
        std::cout << std::setfill(' ') << std::setw(4) << this->m_lines[offset] << " ";
    }

    uint8_t instruction = this->m_code[offset];
    switch (instruction) {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", offset);
    default:
        std::cout << "Unknown opcode " << instruction << std::endl;
        return offset + 1;
    }
}

size_t Chunk::simpleInstruction(const std::string & name, size_t offset) const {
    std::cout << std::setfill(' ') << std::setw(16) << name << std::endl;
    return offset + 1;
}

size_t Chunk::addConstant(const Value & value) {
    this->m_constants.push_back(value);
    return this->m_constants.size() - 1;
}

size_t Chunk::constantInstruction(const std::string & name, size_t offset) const {
    uint8_t constant = this->m_code[offset + 1];
    std::cout << std::setfill(' ') << std::setw(16) << name << std::setw(16) << +constant << " '";
    this->m_constants[constant].print();
    std::cout << "'" << std::endl;
    return offset + 2;
}