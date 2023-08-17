#pragma once

#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
  public:
    Lexer();
    [[nodiscard]] std::vector<Token> tokenize(std::string const & source);

  private:
    void scanToken(std::string const & source);
    [[nodiscard]] bool isAtEnd(std::string const & source) const;
    char advance(std::string const & source);
    void addToken(Token::Type type, std::string const & lexeme);
    [[nodiscard]] bool match(char c, std::string const & source);
    [[nodiscard]] char peek(std::string const & source) const;
    [[nodiscard]] char peekNext(std::string const & source) const;
    void string(std::string const & source);
    [[nodiscard]] bool isDigit(char c) const;
    [[nodiscard]] bool isAlpha(char c) const;
    void number(std::string const & source);
    void identifier(std::string const & source);
    void skipWhitespace(std::string const & source);
    size_t m_start;
    size_t m_current;
    size_t m_line;
    std::vector<Token> m_tokens;
};