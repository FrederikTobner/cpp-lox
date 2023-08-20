#pragma once

#include <cstdint>

namespace cppLox::Frontend {

/// @brief The precedence of the expressions and statements of the language
enum class Precedence : uint8_t {
    NONE,
    ASSIGNMENT, // =
    OR,         // or
    AND,        // and
    EQUALITY,   // == !=
    COMPARISON, // < > <= >=
    TERM,       // + -
    FACTOR,     // * /
    UNARY,      // ! -
    CALL,       // . () []
    PRIMARY
};
} // namespace cppLox::Frontend