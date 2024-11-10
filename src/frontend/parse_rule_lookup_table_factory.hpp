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
#include "parser_traits.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

/// @brief Factory for the parse rules of the Pratt parser.
/// @tparam PARSER The type of the parser / compiler.
template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
class ParseRuleLookupTableFactory {
  public:
    /// @brief Creates the parse rules for the Pratt parser.
    /// @return The created parse rules for the Pratt parser.
    [[nodiscard]] static auto createParseRules()
        -> std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)>;

  private:
    static auto
    initializeGroupingTokens(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) -> void;
    static void initializeSeparators(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
    static void
    initializeArithmeticOperators(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
    static void
    initializeComparisonOperators(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
    static void
    initializeLiteralsAndIdentifiers(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
    static void initializeKeywordsAndLogicalOperators(
        std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
    static void
    initializeStatementKeywords(std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules);
};

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
auto ParseRuleLookupTableFactory<PARSER>::createParseRules()
    -> std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> {
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> rules{};

    initializeGroupingTokens(rules);
    initializeSeparators(rules);
    initializeArithmeticOperators(rules);
    initializeComparisonOperators(rules);
    initializeLiteralsAndIdentifiers(rules);
    initializeKeywordsAndLogicalOperators(rules);
    initializeStatementKeywords(rules);

    rules[static_cast<size_t>(Token::Type::END_OF_FILE)] = ParseRuleBuilder<PARSER>::nullRule();

    return rules;
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
auto ParseRuleLookupTableFactory<PARSER>::initializeGroupingTokens(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) -> void {
    rules[static_cast<size_t>(Token::Type::LEFT_PARENTHESES)] =
        ParseRuleBuilder<PARSER>::fullRule(&PARSER::grouping, &PARSER::call, Precedence::CALL);
    rules[static_cast<size_t>(Token::Type::RIGHT_PARENTHESES)] = ParseRuleBuilder<PARSER>::nullRule();
    rules[static_cast<size_t>(Token::Type::LEFT_BRACE)] = ParseRuleBuilder<PARSER>::nullRule();
    rules[static_cast<size_t>(Token::Type::RIGHT_BRACE)] = ParseRuleBuilder<PARSER>::nullRule();
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeSeparators(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    auto const separators = {Token::Type::COMMA, Token::Type::DOT, Token::Type::SEMICOLON};
    for (auto type : separators) {
        rules[static_cast<size_t>(type)] = ParseRuleBuilder<PARSER>::nullRule();
    }
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeArithmeticOperators(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    rules[static_cast<size_t>(Token::Type::MINUS)] =
        ParseRuleBuilder<PARSER>::fullRule(&PARSER::unary, &PARSER::binary, Precedence::TERM);
    rules[static_cast<size_t>(Token::Type::PLUS)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::TERM);
    rules[static_cast<size_t>(Token::Type::SLASH)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::FACTOR);
    rules[static_cast<size_t>(Token::Type::STAR)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::FACTOR);
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeComparisonOperators(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    rules[static_cast<size_t>(Token::Type::BANG)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::unary);
    rules[static_cast<size_t>(Token::Type::BANG_EQUAL)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::EQUALITY);
    rules[static_cast<size_t>(Token::Type::EQUAL)] = ParseRuleBuilder<PARSER>::nullRule();
    rules[static_cast<size_t>(Token::Type::EQUAL_EQUAL)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::EQUALITY);
    rules[static_cast<size_t>(Token::Type::GREATER)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::COMPARISON);
    rules[static_cast<size_t>(Token::Type::GREATER_EQUAL)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::COMPARISON);
    rules[static_cast<size_t>(Token::Type::LESS)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::COMPARISON);
    rules[static_cast<size_t>(Token::Type::LESS_EQUAL)] =
        ParseRuleBuilder<PARSER>::infixRule(&PARSER::binary, Precedence::COMPARISON);
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeLiteralsAndIdentifiers(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    rules[static_cast<size_t>(Token::Type::IDENTIFIER)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::variable);
    rules[static_cast<size_t>(Token::Type::STRING)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::string);
    rules[static_cast<size_t>(Token::Type::NUMBER)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::number);
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeKeywordsAndLogicalOperators(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    rules[static_cast<size_t>(Token::Type::AND)] = ParseRuleBuilder<PARSER>::infixRule(&PARSER::and_, Precedence::AND);
    rules[static_cast<size_t>(Token::Type::OR)] = ParseRuleBuilder<PARSER>::infixRule(&PARSER::or_, Precedence::OR);
    auto const literals = {Token::Type::FALSE, Token::Type::NULL_, Token::Type::TRUE};
    for (auto type : literals) {
        rules[static_cast<size_t>(type)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::literal);
    }
}

template <typename PARSER>
    requires ParserTraits<PARSER>::hasRequiredMethods
void ParseRuleLookupTableFactory<PARSER>::initializeStatementKeywords(
    std::array<ParseRule<PARSER>, static_cast<size_t>(Token::Type::AMOUNT)> & rules) {
    auto const nullKeyWords = {Token::Type::CLASS, Token::Type::ELSE,   Token::Type::FUN,   Token::Type::FOR,
                               Token::Type::IF,    Token::Type::RETURN, Token::Type::SUPER, Token::Type::THIS,
                               Token::Type::VAR,   Token::Type::WHILE};

    for (auto type : nullKeyWords) {
        rules[static_cast<size_t>(type)] = ParseRuleBuilder<PARSER>::nullRule();
    }
    rules[static_cast<size_t>(Token::Type::PRINT)] = ParseRuleBuilder<PARSER>::prefixRule(&PARSER::printStatement);
}

} // namespace cppLox::Frontend
