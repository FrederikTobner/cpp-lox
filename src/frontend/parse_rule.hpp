#pragma once

#include <optional>
#include <vector>

#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

template <typename T> class ParseRule {
  public:
    ParseRule();
    ParseRule(std::optional<void (T::*)(std::vector<Token> const & tokens)> prefix,
              std::optional<void (T::*)(std::vector<Token> const & tokens)> infix, Precedence precedence);
    ~ParseRule() = default;
    /// @brief Gets the prefix function of the rule.
    /// @return An optional containing the prefix function of the rule.
    [[nodiscard]] auto prefix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)>;
    /// @brief Gets the infix function of the rule.
    /// @return An optional containing the infix function of the rule.
    [[nodiscard]] auto infix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)>;
    /// @brief Gets the precedence of the rule.
    /// @return The precedence of the rule.
    [[nodiscard]] auto precedence() -> Precedence const &;

  private:
    Precedence m_precedence;
    std::optional<void (T::*)(std::vector<Token> const & tokens)> m_prefix;
    std::optional<void (T::*)(std::vector<Token> const & tokens)> m_infix;
};

template <typename T>
ParseRule<T>::ParseRule() : m_prefix(std::nullopt), m_infix(std::nullopt), m_precedence(Precedence::NONE) {
}

template <typename T>
ParseRule<T>::ParseRule(std::optional<void (T::*)(std::vector<Token> const & tokens)> prefix,
                        std::optional<void (T::*)(std::vector<Token> const & tokens)> infix, Precedence precedence)
    : m_prefix(prefix), m_infix(infix), m_precedence(precedence) {
}

/// @brief Gets the prefix function of the rule.
/// @return An optional containing the prefix function of the rule.
template <typename T>
[[nodiscard]] auto ParseRule<T>::prefix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)> {
    return m_prefix;
}
/// @brief Gets the infix function of the rule.
/// @return An optional containing the infix function of the rule.
template <typename T>
[[nodiscard]] auto ParseRule<T>::infix() const -> std::optional<void (T::*)(std::vector<Token> const & tokens)> {
    return m_infix;
}
/// @brief Gets the precedence of the rule.
/// @return The precedence of the rule.
template <typename T> [[nodiscard]] auto ParseRule<T>::precedence() -> Precedence const & {
    return m_precedence;
}

} // namespace cppLox::Frontend