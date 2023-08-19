#include "opcode.hpp"

#include <unordered_map>

#include "assert.hpp"

[[nodiscard]] std::string_view opcode_as_string(const Opcode value) {
    switch (value) {
    case OP_ADD:
        return "OP_ADD";
    case OP_CONSTANT:
        return "OP_CONSTANT";
    case OP_DIVIDE:
        return "OP_DIVIDE";
    case OP_EQUAL:
        return "OP_EQUAL";
    case OP_FALSE:
        return "OP_FALSE";
    case OP_GREATER:
        return "OP_GREATER";
    case OP_GREATER_EQUAL:
        return "OP_GREATER_EQUAL";
    case OP_LESS:
        return "OP_LESS";
    case OP_LESS_EQUAL:
        return "OP_LESS_EQUAL";
    case OP_MULTIPLY:
        return "OP_MULTIPLY";
    case OP_NEGATE:
        return "OP_NEGATE";
    case OP_NOT:
        return "OP_NOT";
    case OP_NOT_EQUAL:
        return "OP_NOT_EQUAL";
    case OP_NULL:
        return "OP_NULL";
    case OP_PRINT:
        return "OP_PRINT";
    case OP_RETURN:
        return "OP_RETURN";
    case OP_SUBTRACT:
        return "OP_SUBTRACT";
    case OP_TRUE:
        return "OP_TRUE";
    }
    return "Unknown opcode";
}

std::ostream & operator<<(std::ostream & out, Opcode value) {
    return out << opcode_as_string(value);
}