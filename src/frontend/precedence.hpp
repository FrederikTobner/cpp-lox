#pragma once

#include <cstdint>

namespace cppLox::Frontend {

/// @brief The precedence of the expressions and statements of the language
enum class Precedence : uint8_t {
    /// @brief The lowest precedence
    NONE,
    /// @brief The precedence of the assignment operator
    ASSIGNMENT, // =
    /// @brief The precedence of the or operator
    OR, // or
    /// @brief The precedence of the and operator
    AND, // and
    /// @brief The precedence of equality operators
    EQUALITY, // == !=
    /// @brief The precedence of comparison operators
    COMPARISON, // < > <= >=
    /// @brief The precedence of term operators
    TERM, // + -
    /// @brief The precedence of factor operators
    FACTOR, // * /
    /// @brief The precedence of unary operators
    UNARY, // ! -
    /// @brief The precedence of call and primary operators
    CALL, // . () []
    /// @brief The precedence of primary operators
    PRIMARY
};
} // namespace cppLox::Frontend