#pragma once

#include <string>
#include <vector>

#include "token.hpp"

/// @brief The lexer of the language.
class Lexer {
  public:
    Lexer();
    [[nodiscard]] auto tokenize(std::string const & source) -> std::vector<Token>;

  private:
    auto scanToken(std::string const & source) -> void;
    [[nodiscard]] auto isAtEnd(std::string const & source) const -> bool;
    auto advance(std::string const & source) -> char;
    auto addToken(Token::Type type, std::string const & lexeme) -> void;
    [[nodiscard]] auto match(char c, std::string const & source) -> bool;
    [[nodiscard]] auto peek(std::string const & source) const -> char;
    [[nodiscard]] auto peekNext(std::string const & source) const -> char;
    auto string(std::string const & source) -> void;
    [[nodiscard]] auto isDigit(char c) const -> bool;
    [[nodiscard]] auto isAlpha(char c) const -> bool;
    auto number(std::string const & source) -> void;
    auto identifier(std::string const & source) -> void;
    auto skipWhitespace(std::string const & source) -> void;
    size_t m_start;
    size_t m_current;
    size_t m_line;
    std::vector<Token> m_tokens;
};