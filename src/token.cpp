#include "token.hpp"

#include <iostream>

Token::Token(Type type, std::string lexeme, size_t line) {
    this->m_type = type;
    this->m_lexeme = lexeme;
    this->m_line = line;
}

[[nodiscard]] Token::Type Token::type() const {
    return this->m_type;
}

[[nodiscard]] std::string Token::lexeme() const {
    return this->m_lexeme;
}

[[nodiscard]] size_t Token::line() const {
    return this->m_line;
}