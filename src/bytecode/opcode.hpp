/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file opcode.hpp
 * @brief This file contains the Opcode enum and the declaration some functions for it.
 */

#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include <string_view>

namespace cppLox::ByteCode {

/// @brief The opcodes of the intermediate language
enum Opcode : uint8_t {
    /// @brief Pops the top two values off the stack, adds them, and pushes the result back on the stack.
    ADD,
    /// @brief Calls the function at the given index on the stack.
    CALL,
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
    /// @brief Gets the value of the local variable with the given index on the stack and pushes it onto the stack.
    GET_LOCAL,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    GREATER,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    GREATER_EQUAL,
    /// @brief Jumps to the given offset.
    JUMP,
    /// @brief Jumps to the given offset if the top value on the stack is false.
    JUMP_IF_FALSE,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    LESS,
    /// @brief Pops the top two values off the stack, compares them, and pushes the result back on the stack.
    LESS_EQUAL,
    /// @brief Adds an offset to the instruction pointer, like JUMP
    LOOP,
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
    /// @brief  Sets the global variable with the given name at the given index on the stack.
    SET_GLOBAL,
    /// @brief Sets the local variable with the given index on the stack.
    SET_LOCAL,
    /// @brief Pops the top two values off the stack, subtracts them, and pushes the result back on the stack.
    SUBTRACT,
    /// @brief Pushes the true value onto the stack.
    TRUE,
    AMOUNT
};

/// @brief Converts the given opcode to a string
/// @param value The opcode to convert.
/// @return The string representation of the given opcode.
[[nodiscard]] auto opcode_as_string(const Opcode value) -> std::string_view;

/// @brief Prints the given opcode to the given output stream.
/// @param os The output stream to print to.
/// @param opcode The opcode to print.
/// @return The output stream.
auto operator<<(std::ostream & os, cppLox::ByteCode::Opcode const & opcode) -> std::ostream &;

} // namespace cppLox::ByteCode
