#pragma once

#include <format>
#include <ostream>
#include <string>

#include <string_view>

namespace cppLox::ByteCode {

/// @brief The opcodes of the intermediate language
enum Opcode {
    ADD,
    CONSTANT,
    DIVIDE,
    EQUAL,
    FALSE,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    MULTIPLY,
    NEGATE,
    NOT,
    NOT_EQUAL,
    NULL_,
    PRINT,
    RETURN,
    SUBTRACT,
    TRUE
};

auto opcode_as_string(const Opcode value) -> std::string_view;

} // namespace cppLox::ByteCode
