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
 * @file parse_rule_lookup_table_factory.hpp
 * @brief This file contains the ParseRuleLookupTableFactory class.
 */

#pragma once

#include <array>

#include "parse_rule.hpp"
#include "parse_rule_builder.hpp"
#include "prattparser_traits.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

/// @brief Factory for the parse rules of the Pratt parser.
/// @tparam T The type of the parser / compiler.
template <typename T>
    requires PrattParserTraits<T>::hasRequiredMethods
class ParseRuleLookupTableFactory {
  public:
    /// @brief Creates the parse rules for the Pratt parser.
    /// @return The created parse rules for the Pratt parser.
    [[nodiscard]] static auto createParseRules() -> std::array<ParseRule<T>, static_cast<size_t>(Token::Type::AMOUNT)> {
        std::array<ParseRule<T>, static_cast<size_t>(Token::Type::AMOUNT)> rules{};

        // Grouping tokens
        rules[static_cast<size_t>(Token::Type::LEFT_PARENTHESES)] =
            ParseRuleBuilder<T>::fullRule(&T::grouping, &T::call, Precedence::CALL);
        rules[static_cast<size_t>(Token::Type::RIGHT_PARENTHESES)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::LEFT_BRACE)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::RIGHT_BRACE)] = ParseRuleBuilder<T>::nullRule();

        // Separators
        rules[static_cast<size_t>(Token::Type::COMMA)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::DOT)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::SEMICOLON)] = ParseRuleBuilder<T>::nullRule();

        // Arithmetic operators
        rules[static_cast<size_t>(Token::Type::MINUS)] =
            ParseRuleBuilder<T>::fullRule(&T::unary, &T::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::PLUS)] = ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::SLASH)] = ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::FACTOR);
        rules[static_cast<size_t>(Token::Type::STAR)] = ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::FACTOR);

        // Comparison operators
        rules[static_cast<size_t>(Token::Type::BANG)] = ParseRuleBuilder<T>::prefixRule(&T::unary);
        rules[static_cast<size_t>(Token::Type::BANG_EQUAL)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::EQUAL)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::EQUAL_EQUAL)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::GREATER)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::GREATER_EQUAL)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS_EQUAL)] =
            ParseRuleBuilder<T>::infixRule(&T::binary, Precedence::COMPARISON);

        // Literals and identifiers
        rules[static_cast<size_t>(Token::Type::IDENTIFIER)] = ParseRuleBuilder<T>::prefixRule(&T::variable);
        rules[static_cast<size_t>(Token::Type::STRING)] = ParseRuleBuilder<T>::prefixRule(&T::string);
        rules[static_cast<size_t>(Token::Type::NUMBER)] = ParseRuleBuilder<T>::prefixRule(&T::number);

        // Keywords and logical operators
        rules[static_cast<size_t>(Token::Type::AND)] = ParseRuleBuilder<T>::infixRule(&T::and_, Precedence::AND);
        rules[static_cast<size_t>(Token::Type::OR)] = ParseRuleBuilder<T>::infixRule(&T::or_, Precedence::OR);
        rules[static_cast<size_t>(Token::Type::FALSE)] = ParseRuleBuilder<T>::prefixRule(&T::literal);
        rules[static_cast<size_t>(Token::Type::NULL_)] = ParseRuleBuilder<T>::prefixRule(&T::literal);
        rules[static_cast<size_t>(Token::Type::TRUE)] = ParseRuleBuilder<T>::prefixRule(&T::literal);

        // Statement keywords
        rules[static_cast<size_t>(Token::Type::CLASS)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::ELSE)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::FUN)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::FOR)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::IF)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::PRINT)] = ParseRuleBuilder<T>::prefixRule(&T::printStatement);
        rules[static_cast<size_t>(Token::Type::RETURN)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::SUPER)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::THIS)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::VAR)] = ParseRuleBuilder<T>::nullRule();
        rules[static_cast<size_t>(Token::Type::WHILE)] = ParseRuleBuilder<T>::nullRule();

        // Special tokens
        rules[static_cast<size_t>(Token::Type::END_OF_FILE)] = ParseRuleBuilder<T>::nullRule();

        return rules;
    }
};
} // namespace cppLox::Frontend
