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
/// @tparam PARSER The type of the parser / compiler.
template <typename PARSER> class ParseRuleBuilder {
  public:
    /// @brief Creates a null rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto nullRule() -> ParseRule<PARSER>;

    /// @brief Creates a prefix rule.
    /// @param prefix The prefix function of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto prefixRule(void (PARSER::*prefix)(std::vector<Token> const &, bool)) -> ParseRule<PARSER>;

    /// @brief Creates an infix rule.
    /// @param infix The infix function of the rule.
    /// @param prec The precedence of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto infixRule(void (PARSER::*infix)(std::vector<Token> const &), Precedence prec)
        -> ParseRule<PARSER>;

    /// @brief Creates a full rule.
    /// @param prefix The prefix function of the rule.
    /// @param infix The infix function of the rule.
    /// @param prec The precedence of the rule.
    /// @return The created parse rule.
    [[nodiscard]] static auto fullRule(void (PARSER::*prefix)(std::vector<Token> const &, bool),
                                       void (PARSER::*infix)(std::vector<Token> const &), Precedence prec)
        -> ParseRule<PARSER>;
};

template <typename PARSER> auto ParseRuleBuilder<PARSER>::nullRule() -> ParseRule<PARSER> {
    return ParseRule<PARSER>(std::nullopt, std::nullopt, Precedence::NONE);
}

template <typename PARSER>
auto ParseRuleBuilder<PARSER>::prefixRule(void (PARSER::*prefix)(std::vector<Token> const &, bool))
    -> ParseRule<PARSER> {
    return ParseRule<PARSER>(
        [prefix](PARSER * parser, auto & tokens, bool canAssign) { std::invoke(prefix, parser, tokens, canAssign); },
        std::nullopt, Precedence::NONE);
}

template <typename PARSER>
auto ParseRuleBuilder<PARSER>::infixRule(void (PARSER::*infix)(std::vector<Token> const &), Precedence prec)
    -> ParseRule<PARSER> {
    return ParseRule<PARSER>(
        std::nullopt, [infix](PARSER * parser, auto & tokens) { std::invoke(infix, parser, tokens); }, prec);
}

template <typename PARSER>
auto ParseRuleBuilder<PARSER>::fullRule(void (PARSER::*prefix)(std::vector<Token> const &, bool),
                                        void (PARSER::*infix)(std::vector<Token> const &), Precedence prec)
    -> ParseRule<PARSER> {
    return ParseRule<PARSER>(
        [prefix](PARSER * parser, auto & tokens, bool canAssign) { std::invoke(prefix, parser, tokens, canAssign); },
        [infix](PARSER * parser, auto & tokens) { std::invoke(infix, parser, tokens); }, prec);
}

} // namespace cppLox::Frontend