#pragma once

#include <string>
#include <vector>

#include "token.hpp"

namespace cppLox::Frontend {

/// @brief The lexer of the language.
class Lexer {
  public:
    /// @brief Default constructor.
    Lexer() = default;
    /// @brief Default destructor.
    ~Lexer() = default;

    /// @brief Tokenizes the given source code.
    [[nodiscard]] auto tokenize(std::string const & source) -> std::vector<Token>;

  private:
    /// @brief Scans the next token.
    auto scanToken(std::string const & source) -> void;

    /// @brief Checks if the lexer is at the end of the source code.
    [[nodiscard]] auto isAtEnd(std::string const & source) const -> bool;

    /// @brief Advances the lexer by one character.
    auto advance(std::string const & source) -> char;

    /// @brief Adds a token to the token list.
    auto addToken(Token::Type type, std::string const & lexeme) -> void;

    /// @brief Checks if the current character matches the given character.
    /// @param c The character to match.
    /// @param source The source code.
    /// @return True if the current character matches the given character, false otherwise.
    [[nodiscard]] auto match(char c, std::string const & source) -> bool;

    /// @brief Peeks at the current character.
    /// @param source The source code.
    /// @return The current character.
    [[nodiscard]] auto peek(std::string const & source) const -> char;

    /// @brief Peeks at the next character.
    /// @param source The source code.
    /// @return The next character.
    [[nodiscard]] auto peekNext(std::string const & source) const -> char;

    /// @brief Scans a string.
    /// @param source The source code that is being scanned.
    /// @return The scanned string.
    auto string(std::string const & source) -> void;

    /// @brief Checks if the given character is a digit.
    /// @param c The character to check.
    /// @return True if the given character is a digit, false otherwise.
    [[nodiscard]] auto isDigit(char c) const -> bool;

    /// @brief Checks if the given character is a letter.
    /// @param c The character to check.
    /// @return True if the given character is a letter, false otherwise.
    [[nodiscard]] auto isAlpha(char c) const -> bool;

    /// @brief Parses a number.
    /// @param source The source code that is being scanned.
    auto number(std::string const & source) -> void;

    /// @brief Parses an identifier.
    /// @param source The source code that is being scanned.
    auto identifier(std::string const & source) -> void;

    /// @brief Skips all whitespace characters.
    /// @param source The source code that is being scanned.
    auto skipWhitespace(std::string const & source) -> void;

    /// @brief The start of the token that is currently being scanned.
    size_t m_start;

    /// @brief The current character that is being scanned.
    size_t m_current;

    /// @brief The current line number.
    size_t m_line;

    /// @brief The list of tokens that have been created during the lexical analysis.
    std::vector<Token> m_tokens;
};

} // namespace cppLox::Frontend