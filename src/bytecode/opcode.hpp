#pragma once

#include <format>
#include <iostream>
#include <string>

#include <string_view>

namespace cppLox::ByteCode {

/// @brief The opcodes of the intermediate language
enum Opcode {
    /// @brief Pops the top two values off the stack, adds them, and pushes the result back on the stack.
    ADD,
    /// @brief Pushes the given constant onto the stack.
    CONSTANT,
    /// @brief Defines a global variable
    DEFINE_GLOBAL,
    /// @brief Pops the top two values off the stack, divides them, and pushes the result back on the stack.
    DIVIDE,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    EQUAL,
    /// @brief Pushes the false value onto the stack.
    FALSE,
    /// @brief Gets the value of the global variable with the name of the given index on the stack and pushes it onto
    /// the stack.
    GET_GLOBAL,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    GREATER,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    GREATER_EQUAL,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    LESS,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    LESS_EQUAL,
    /// @brief Pops the top two values off the stack, multiplies them, and pushes the result back on the stack.
    MULTIPLY,
    /// @brief Pops the top value off the stack, negates it, and pushes the result back on the stack.
    NEGATE,
    /// @brief Pops the top value off the stack, negates it, and pushes the result back on the stack.
    NOT,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    NOT_EQUAL,
    /// @brief Pushes the null value onto the stack.
    NULL_,
    /// @brief Pops the top value off the stack.
    POP,
    /// @brief Pops the top value off the stack and prints it.
    PRINT,
    /// @brief Pops the top value off the stack and returns it.
    RETURN,
    /// @brief  Sets the global variable with the given name at the given index from the stack.
    SET_GLOBAL,
    /// @brief Pops the top two values off the stack, subtracts them, and pushes the result back on the stack.
    SUBTRACT,
    /// @brief Pushes the true value onto the stack.
    TRUE
};

[[nodiscard]] auto opcode_as_string(const Opcode value) -> std::string_view;

auto operator<<(std::ostream & os, cppLox::ByteCode::Opcode const & opcode) -> std::ostream &;

} // namespace cppLox::ByteCode
