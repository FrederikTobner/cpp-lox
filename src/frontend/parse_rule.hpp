#pragma once

#include <optional>
#include <vector>

#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

/// @brief A parserule for the Pratt parser.
/// @tparam T The type of the parser / compiler.
template <typename T> class ParseRule {
  public:
    /// @brief Trivial constructor of the parse rule.
    ParseRule();

    /// @brief Constructs a new parse rule.
    ParseRule(std::optional<void (T::*)(std::vector<Token> const & tokens, bool canAssign)> prefix,
              std::optional<void (T::*)(std::vector<Token> const & tokens)> infix, Precedence precedence);

    /// @brief Destructor of the parse rule.
    ~ParseRule() = default;

    /// @brief Gets the prefix function of the rule.
    /// @return An optional containing the prefix function of the rule.
    [[nodiscard]] auto prefix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens, bool canAssign)>;

    /// @brief Gets the infix function of the rule.
    /// @return An optional containing the infix function of the rule.
    [[nodiscard]] auto infix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)>;

    /// @brief Gets the precedence of the rule.
    /// @return The precedence of the rule.
    [[nodiscard]] auto precedence() -> Precedence const &;

  private:
    /// @brief The precedence of the rule.
    Precedence m_precedence;

    /// @brief The prefix function of the rule.
    std::optional<void (T::*)(std::vector<Token> const & tokens, bool canAssign)> m_prefix;

    /// @brief The infix function of the rule.
    std::optional<void (T::*)(std::vector<Token> const & tokens)> m_infix;
};

template <typename T>
ParseRule<T>::ParseRule() : m_prefix(std::nullopt), m_infix(std::nullopt), m_precedence(Precedence::NONE) {
}

template <typename T>
ParseRule<T>::ParseRule(std::optional<void (T::*)(std::vector<Token> const & tokens, bool canAssign)> prefix,
                        std::optional<void (T::*)(std::vector<Token> const & tokens)> infix, Precedence precedence)
    : m_prefix(prefix), m_infix(infix), m_precedence(precedence) {
}

template <typename T>
[[nodiscard]] auto ParseRule<T>::prefix() const
    -> std::optional<void (T::*)(std::vector<Token> const & tokens, bool canAssign)> {
    return m_prefix;
}

template <typename T>
[[nodiscard]] auto ParseRule<T>::infix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)> {
    return m_infix;
}

template <typename T> [[nodiscard]] auto ParseRule<T>::precedence() -> Precedence const & {
    return m_precedence;
}

} // namespace cppLox::Frontend