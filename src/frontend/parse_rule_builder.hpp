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
 * @file parse_rule_builder.hpp
 * @brief This file contains the parse rule builder for the Pratt parser.
 */

#pragma once
#include "parse_rule.hpp"

namespace cppLox::Frontend {

/// @brief A builder for parse rules.
/// @tparam T The type of the parser / compiler.
template <typename T> class ParseRuleBuilder {
  public:
    /// @brief Creates a null rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto nullRule() -> ParseRule<T> {
        return ParseRule<T>(std::nullopt, std::nullopt, Precedence::NONE);
    }

    /// @brief Creates a prefix rule.
    /// @param prefix The prefix function of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto prefixRule(void (T::*prefix)(std::vector<Token> const &, bool)) -> ParseRule<T> {
        return ParseRule<T>(
            [prefix](T * instance, auto & tokens, bool canAssign) { std::invoke(prefix, instance, tokens, canAssign); },
            std::nullopt, Precedence::NONE);
    }

    /// @brief Creates an infix rule.
    /// @param infix The infix function of the rule.
    /// @param prec The precedence of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto infixRule(void (T::*infix)(std::vector<Token> const &), Precedence prec) -> ParseRule<T> {
        return ParseRule<T>(
            std::nullopt, [infix](T * instance, auto & tokens) { std::invoke(infix, instance, tokens); }, prec);
    }

    /// @brief Creates a full rule.
    /// @param prefix The prefix function of the rule.
    /// @param infix The infix function of the rule.
    /// @param prec The precedence of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto fullRule(void (T::*prefix)(std::vector<Token> const &, bool),
                                       void (T::*infix)(std::vector<Token> const &), Precedence prec) -> ParseRule<T> {
        return ParseRule<T>(
            [prefix](T * instance, auto & tokens, bool canAssign) { std::invoke(prefix, instance, tokens, canAssign); },
            [infix](T * instance, auto & tokens) { std::invoke(infix, instance, tokens); }, prec);
    }
};

} // namespace cppLox::Frontend