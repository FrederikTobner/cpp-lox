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
    using PREFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &, bool);
    using INFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &);

    template <auto METHOD>
    static constexpr bool checkPrefixSignature = std::is_same_v<decltype(METHOD), PREFIX_SIGNATURE>;

    template <auto METHOD>
    static constexpr bool checkInfixSignature = std::is_same_v<decltype(METHOD), INFIX_SIGNATURE>;

    static constexpr bool checkPrefixMethods() {
        static_assert(
            requires { &PARSER::grouping; } && checkPrefixSignature<&PARSER::grouping>,
            "Method grouping must exist and have signature: void(std::vector<Token> const &, bool)");

        static_assert(
            requires { &PARSER::variable; } && checkPrefixSignature<&PARSER::variable>,
            "Method variable must exist and have signature: void(std::vector<Token> const &, bool)");

        static_assert(
            requires { &PARSER::string; } && checkPrefixSignature<&PARSER::string>,
            "Method string must exist and have signature: void(std::vector<Token> const &, bool)");

        static_assert(
            requires { &PARSER::number; } && checkPrefixSignature<&PARSER::number>,
            "Method number must exist and have signature: void(std::vector<Token> const &, bool)");

        static_assert(
            requires { &PARSER::literal; } && checkPrefixSignature<&PARSER::literal>,
            "Method literal must exist and have signature: void(std::vector<Token> const &, bool)");

        return true;
    }

    static constexpr bool checkInfixMethods() {
        static_assert(
            requires { &PARSER::call; } && checkInfixSignature<&PARSER::call>,
            "Method call must exist and have signature: void(std::vector<Token> const &)");

        static_assert(
            requires { &PARSER::binary; } && checkInfixSignature<&PARSER::binary>,
            "Method binary must exist and have signature: void(std::vector<Token> const &)");

        static_assert(
            requires { &PARSER::and_; } && checkInfixSignature<&PARSER::and_>,
            "Method and_ must exist and have signature: void(std::vector<Token> const &)");

        static_assert(
            requires { &PARSER::or_; } && checkInfixSignature<&PARSER::or_>,
            "Method or_ must exist and have signature: void(std::vector<Token> const &)");

        return true;
    }

    static constexpr bool checkForRequiredMethods() {
        static_assert(std::is_class_v<PARSER>, "Parser type must be a class");
        static_assert(checkPrefixMethods(), "Prefix method checks failed");
        static_assert(checkInfixMethods(), "Infix method checks failed");
        return true;
    }

    static constexpr bool hasRequiredMethods = checkForRequiredMethods();
};

} // namespace cppLox::Frontend
