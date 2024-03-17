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
 * @file precedence.hpp
 * @brief This file contains the Precedence enum.
 */

#pragma once

#include <cstdint>

namespace cppLox::Frontend {

/// @brief The precedence of the expressions and statements of the language
enum class Precedence : uint8_t {
    /// @brief The lowest precedence, default value
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