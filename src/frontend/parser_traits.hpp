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
 * @file parser_traits.hpp
 * @brief This file contains the type traits for Pratt parsers.
 */

#pragma once

#include <concepts>
#include <vector>

#include "token.hpp"

namespace cppLox::Frontend {

/// @brief Checks if the given type has the required methods for a Pratt parser.
/// @tparam PARSER The type to check.
template <typename PARSER> struct ParserTraits {

    /// @brief The signature for a prefix rule.
    using PREFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &, bool);

    /// @brief The signature for an infix rule.
    using INFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &);

    /// @brief Checks if the given type has the required method with the correct signature for a prefix rule.
    /// @tparam PARSER The type to check.
    /// @tparam METHOD The method to check.
    /// @return True if the method has the correct signature, false otherwise.
    template <auto METHOD>
    static constexpr bool checkPrefixSignature = std::is_same_v<decltype(METHOD), PREFIX_SIGNATURE>;

    /// @brief Checks if the given type has the required method with the correct signature for an infix rule.
    /// @tparam PARSER The type to check.
    /// @tparam METHOD The method to check.
    /// @return True if the method has the correct signature, false otherwise.
    template <auto METHOD>
    static constexpr bool checkInfixSignature = std::is_same_v<decltype(METHOD), INFIX_SIGNATURE>;

    /// @brief Checks if the given type has the required methods for handling all prefix rules.
    /// @return True if the type has the required methods, false otherwise.
    static constexpr bool checkPrefixMethods() {
        if constexpr (!requires { &PARSER::grouping; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: grouping(std::vector<Token> const &, bool)");
        } else if constexpr (!checkPrefixSignature<&PARSER::grouping>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for grouping: expected void(std::vector<Token> const &, bool)");
        }

        if constexpr (!requires { &PARSER::variable; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: variable(std::vector<Token> const &, bool)");
        } else if constexpr (!checkPrefixSignature<&PARSER::variable>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for variable: expected void(std::vector<Token> const &, bool)");
        }

        if constexpr (!requires { &PARSER::string; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: string(std::vector<Token> const &, bool)");
        } else if constexpr (!checkPrefixSignature<&PARSER::string>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for string: expected void(std::vector<Token> const &, bool)");
        }

        if constexpr (!requires { &PARSER::number; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: number(std::vector<Token> const &, bool)");
        } else if constexpr (!checkPrefixSignature<&PARSER::number>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for number: expected void(std::vector<Token> const &, bool)");
        }

        if constexpr (!requires { &PARSER::literal; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: literal(std::vector<Token> const &, bool)");
        } else if constexpr (!checkPrefixSignature<&PARSER::literal>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for literal: expected void(std::vector<Token> const &, bool)");
        }

        return true;
    }

    /// @brief Checks if the given type has the required methods for handling all infix rules.
    /// @return True if the type has the required methods, false otherwise.
    static constexpr bool checkInfixMethods() {
        if constexpr (!requires { &PARSER::call; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: call(std::vector<Token> const &)");
        } else if constexpr (!checkInfixSignature<&PARSER::call>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for call: expected void(std::vector<Token> const &)");
        }

        if constexpr (!requires { &PARSER::binary; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: binary(std::vector<Token> const &)");
        } else if constexpr (!checkInfixSignature<&PARSER::binary>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for binary: expected void(std::vector<Token> const &)");
        }

        if constexpr (!requires { &PARSER::and_; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: and_(std::vector<Token> const &)");
        } else if constexpr (!checkInfixSignature<&PARSER::and_>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for and_: expected void(std::vector<Token> const &)");
        }

        if constexpr (!requires { &PARSER::or_; }) {
            static_assert(sizeof(PARSER) == 0, "Missing required method: or_(std::vector<Token> const &)");
        } else if constexpr (!checkInfixSignature<&PARSER::or_>) {
            static_assert(sizeof(PARSER) == 0,
                          "Incorrect signature for or_: expected void(std::vector<Token> const &)");
        }

        return true;
    }

    /// @brief Checks if the given type has the required methods for a Pratt parser.
    /// @return True if the type has the required methods, false otherwise.
    static constexpr bool check() {
        static_assert(std::is_class_v<PARSER>, "Parser type must be a class");
        static_assert(checkPrefixMethods(), "Prefix method checks failed");
        static_assert(checkInfixMethods(), "Infix method checks failed");
        return true;
    }

    static constexpr bool hasRequiredMethods = check();
};

} // namespace cppLox::Frontend
