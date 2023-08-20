#include "token.hpp"

#include <format>
#include <iostream>

Token::Token(Type type, std::string lexeme, size_t line) {
    this->m_type = type;
    this->m_lexeme = lexeme;
    this->m_line = line;
}

auto operator<<(std::ostream & os, Token const & token) -> std::ostream & {
    os << std::format("Token({}, {})", token.m_lexeme, token.m_line);
    return os;
}