#pragma once

#include <string>

class Token {
  public:
    /// @brief The possible types of a token
    enum Type {
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
    };
    /// @brief Construct a new Token object
    /// @param type The type of the token
    /// @param lexeme The string representation of the token
    /// @param line The line number where the token was found
    Token(Type type, std::string lexeme, size_t line);

    /// @brief Appends the string representation of the token to the output stream
    /// @param os The output stream to append to
    /// @param token The token to append
    /// @return The output stream
    friend std::ostream & operator<<(std::ostream & os, Token const & token);

    /// @brief Get the type of the token
    /// @return The type of the token
    [[nodiscard]] Type type() const {
        return this->m_type;
    }

    /// @brief Get the string representation of the token
    /// @return The string representation of the token
    [[nodiscard]] std::string const & lexeme() const {
        return this->m_lexeme;
    }

    /// @brief Get the line number where the token was found
    /// @return The line number where the token was found
    [[nodiscard]] inline size_t line() const {
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