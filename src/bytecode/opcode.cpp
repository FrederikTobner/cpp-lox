#include "opcode.hpp"

#include <unordered_map>

[[nodiscard]] auto cppLox::ByteCode::opcode_as_string(const cppLox::ByteCode::Opcode value) -> std::string_view {
    switch (value) {
    case Opcode::ADD:
        return "ADD";
    case Opcode::CONSTANT:
        return "CONSTANT";
    case Opcode::DIVIDE:
        return "DIVIDE";
    case Opcode::EQUAL:
        return "EQUAL";
    case Opcode::FALSE:
        return "FALSE";
    case Opcode::GREATER:
        return "GREATER";
    case Opcode::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case Opcode::LESS:
        return "LESS";
    case Opcode::LESS_EQUAL:
        return "LESS_EQUAL";
    case Opcode::MULTIPLY:
        return "MULTIPLY";
    case Opcode::NEGATE:
        return "NEGATE";
    case Opcode::NOT:
        return "NOT";
    case Opcode::NOT_EQUAL:
        return "NOT_EQUAL";
    case Opcode::NULL_:
        return "NULL_";
    case Opcode::PRINT:
        return "PRINT";
    case Opcode::RETURN:
        return "RETURN";
    case Opcode::SUBTRACT:
        return "SUBTRACT";
    case Opcode::TRUE:
        return "TRUE";
    }
    return "Unknown opcode";
}

auto cppLox::ByteCode::operator<<(std::ostream & os, Opcode const & opcode) -> std::ostream & {
    return os << cppLox::ByteCode::opcode_as_string(opcode);
}
