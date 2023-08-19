#pragma once

#include <format>
#include <ostream>
#include <string>

#include <string_view>

/// @brief The opcodes of the intermediate language
enum class Opcode : uint8_t {
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

auto operator<<(std::ostream & out, Opcode value) -> std::ostream &;

/// @brief Formatter for the Opcode enum
template <> struct std::formatter<Opcode> : std::formatter<std::string> {
    [[nodiscard]] inline auto format(Opcode opcode, format_context & ctx) const {
        return formatter<string>::format(std::format("{}", opcode_as_string(opcode)), ctx);
    }
};