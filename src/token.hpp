#include <string>

class Token {
  public:
    enum Type {
        AND,
        BANG,
        BANG_EQUAL,
        CLASS,
        COMMA,
        DOT,
        ELSE,
        END_OF_FILE,
        EQUAL,
        EQUAL_EQUAL,
        FALSE,
        FUN,
        FOR,
        GREATER,
        GREATER_EQUAL,
        IF,
        IDENTIFIER,
        LEFT_PARENTHESES,
        LEFT_BRACE,
        LESS,
        LESS_EQUAL,
        MINUS,
        RIGHT_PARENTHESES,
        RIGHT_BRACE,
        NUMBER,
        PLUS,
        NULL_,
        OR,
        PRINT,
        RETURN,
        SEMICOLON,
        SLASH,
        STAR,
        STRING,
        SUPER,
        THIS,
        TRUE,
        VAR,
        WHILE,
    };
    /// @brief Construct a new Token object
    /// @param type The type of the token
    /// @param lexeme The string representation of the token
    /// @param line The line number where the token was found
    Token(Type type, std::string lexeme, size_t line);
    [[nodiscard]] Type type() const;
    [[nodiscard]] std::string lexeme() const;
    [[nodiscard]] size_t line() const;

  private:
    Type m_type;
    std::string m_lexeme;
    size_t m_line;
};