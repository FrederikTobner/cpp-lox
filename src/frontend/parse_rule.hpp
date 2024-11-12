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
 * @file parse_rule.hpp
 * @brief This file contains the ParseRule class.
 */

#pragma once

#include <functional>
#include <optional>
#include <vector>

#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

/// @brief A parserule for the Pratt parser.
/// @tparam PARSER The type of the parser.
template <typename PARSER> class ParseRule {

  public:
    /// @brief Trivial constructor of the parse rule.
    ParseRule();

    /// @brief Constructs a new parse rule.
    ParseRule(std::optional<std::function<void(PARSER *, std::vector<Token> const &, bool)>> prefix,
              std::optional<std::function<void(PARSER *, std::vector<Token> const &)>> infix, Precedence precedence);

    /// @brief Destructor of the parse rule.
    ~ParseRule() = default;

    /// @brief Gets the prefix function of the rule.
    /// @return An optional containing the prefix function of the rule.
    [[nodiscard]] auto prefix() const -> std::optional<std::function<void(PARSER *, std::vector<Token> const &, bool)>>;

    /// @brief Gets the infix function of the rule.
    /// @return An optional containing the infix function of the rule.
    [[nodiscard]] auto infix() const -> std::optional<std::function<void(PARSER *, std::vector<Token> const &)>>;

    /// @brief Gets the precedence of the rule.
    /// @return The precedence of the rule.
    [[nodiscard]] auto precedence() -> Precedence const &;

  private:
    /// @brief The precedence of the rule.
    Precedence m_precedence;

    /// @brief The prefix function of the rule.
    std::optional<std::function<void(PARSER *, std::vector<Token> const &, bool)>> m_prefix;

    /// @brief The infix function of the rule.
    std::optional<std::function<void(PARSER *, std::vector<Token> const &)>> m_infix;
};

template <typename PARSER>
ParseRule<PARSER>::ParseRule() : m_prefix(std::nullopt), m_infix(std::nullopt), m_precedence(Precedence::NONE) {
}

template <typename PARSER>
ParseRule<PARSER>::ParseRule(std::optional<std::function<void(PARSER *, std::vector<Token> const &, bool)>> prefix,
                             std::optional<std::function<void(PARSER *, std::vector<Token> const &)>> infix,
                             Precedence precedence)
    : m_prefix(prefix), m_infix(infix), m_precedence(precedence) {
}

template <typename PARSER>
[[nodiscard]] auto ParseRule<PARSER>::prefix() const
    -> std::optional<std::function<void(PARSER *, std::vector<Token> const &, bool)>> {
    return m_prefix;
}

template <typename PARSER>
[[nodiscard]] auto ParseRule<PARSER>::infix() const
    -> std::optional<std::function<void(PARSER *, std::vector<Token> const &)>> {
    return m_infix;
}

template <typename PARSER> [[nodiscard]] auto ParseRule<PARSER>::precedence() -> Precedence const & {
    return m_precedence;
}

} // namespace cppLox::Frontend