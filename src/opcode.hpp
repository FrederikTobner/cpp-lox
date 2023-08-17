#pragma once

#include <format>
#include <ostream>
#include <string>

/// @brief The opcodes of the intermidiate language
typedef enum {
    OP_ADD,
    OP_CONSTANT,
    OP_DIVIDE,
    OP_EQUAL,
    OP_FALSE,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_MULTIPLY,
    OP_NEGATE,
    OP_NOT,
    OP_NOT_EQUAL,
    OP_NULL,
    OP_PRINT,
    OP_RETURN,
    OP_SUBTRACT,
    OP_TRUE,
    // Amount of opcodes in the enum has to be last entry
    COUNT
} Opcode;

std::string const & opcode_as_string(Opcode value);

std::ostream & operator<<(std::ostream & out, Opcode value);

template <> struct std::formatter<Opcode> : std::formatter<std::string> {
    auto format(Opcode opcode, format_context & ctx) const {
        return formatter<string>::format(std::format("{}", opcode_as_string(opcode)), ctx);
    }
};