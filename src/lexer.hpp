#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
  public:
    Lexer();
    [[nodiscard]] std::vector<Token> tokenize(std::string & source);

  private:
    void scanToken(std::string & source);
    [[nodiscard]] bool isAtEnd(std::string & source);
    char advance(std::string & source);
    void addToken(Token::Type type, std::string & lexeme);
    [[nodiscard]] bool match(char c, std::string & source);
    [[nodiscard]] char peek(std::string & source);
    [[nodiscard]] char peekNext(std::string & source);
    void string(std::string & source);
    [[nodiscard]] bool isDigit(char c) const;
    [[nodiscard]] bool isAlpha(char c) const;
    void number(std::string & source);
    void identifier(std::string & source);
    void skipWhitespace(std::string & source);
    size_t m_start;
    size_t m_current;
    size_t m_line;
    std::vector<Token> m_tokens;
};