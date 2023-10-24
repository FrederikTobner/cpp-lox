#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace cppLox::Frontend {

/// @brief Represents a token in the source code
class Token {
  public:
    /// @brief The possible types of a token
    enum class Type : uint8_t {
        /// @brief An and keyword
        AND,
        /// @brief A bang operator
        BANG,
        /// @brief A bang equal operator
        BANG_EQUAL,
        /// @brief A class keyword
        CLASS,
        /// @brief A comma
        COMMA,
        /// @brief A dot
        DOT,
        /// @brief An else keyword
        ELSE,
        /// @brief An end of a file - used to indicate the end of the token stream
        END_OF_FILE,
        /// @brief An equal operator
        EQUAL,
        /// @brief An equal equal operator
        EQUAL_EQUAL,
        /// @brief A false keyword
        FALSE,
        /// @brief A fun keyword
        FUN,
        /// @brief A for keyword
        FOR,
        /// @brief A greater than operator
        GREATER,
        /// @brief A greater than or equal operator
        GREATER_EQUAL,
        /// @brief An if keyword
        IF,
        /// @brief An identifier
        IDENTIFIER,
        /// @brief A left parentheses
        LEFT_PARENTHESES,
        /// @brief A left brace
        LEFT_BRACE,
        /// @brief A less than operator
        LESS,
        /// @brief A less than or equal operator
        LESS_EQUAL,
        /// @brief A minus operator
        MINUS,
        /// @brief A right parentheses
        RIGHT_PARENTHESES,
        /// @brief A right brace
        RIGHT_BRACE,
        /// @brief A number
        NUMBER,
        /// @brief A plus operator
        PLUS,
        /// @brief A null keyword
        NULL_,
        /// @brief An or operator
        OR,
        /// @brief A print keyword
        PRINT,
        /// @brief A return keyword
        RETURN,
        /// @brief A semicolon
        SEMICOLON,
        /// @brief A slash operator
        SLASH,
        /// @brief A star operator
        STAR,
        /// @brief A string
        STRING,
        /// @brief A super keyword
        SUPER,
        /// @brief A this keyword
        THIS,
        /// @brief A true keyword
        TRUE,
        /// @brief A var keyword
        VAR,
        /// @brief A while keyword
        WHILE,
        /// @brief The number of token types - used for initializing the array of parse rules
        AMOUNT
    };
    /// @brief Construct a new Token object
    /// @param type The type of the token
    /// @param lexeme The string representation of the token
    /// @param line The line number where the token was found
    Token(Token::Type type, std::string lexeme, std::size_t line);

    /// @brief Destroy the Token object
    ~Token() = default;

    /// @brief Appends the string representation of the token to the output stream
    /// @param os The output stream to append to
    /// @param token The token to append
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Token const & token) -> std::ostream & {
        os << std::format("Token({}, {})", token.m_lexeme, token.m_line);
        return os;
    }

    /// @brief Get the type of the token
    /// @return The type of the token
    [[nodiscard]] inline auto type() const -> Type {
        return this->m_type;
    }

    /// @brief Get the string representation of the token
    /// @return The string representation of the token
    [[nodiscard]] inline auto lexeme() const -> std::string const & {
        return this->m_lexeme;
    }

    /// @brief Get the line number where the token was found
    /// @return The line number where the token was found
    [[nodiscard]] inline auto line() const -> size_t {
        return this->m_line;
    }

  private:
    /// @brief The type of the token
    Type m_type;

    /// @brief The string representation of the token
    std::string m_lexeme;

    /// @brief The line number where the token was found
    size_t m_line;
};
} // namespace cppLox::Frontend