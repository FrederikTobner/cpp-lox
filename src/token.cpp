#include "token.hpp"

#include <format>
#include <iostream>

Token::Token(Type type, std::string lexeme, size_t line) {
    this->m_type = type;
    this->m_lexeme = lexeme;
    this->m_line = line;
}

std::ostream & operator<<(std::ostream & os, Token const & token) {
    os << std::format("Token({0}, {1}, {2})", unsigned(token.m_type), token.m_lexeme, token.m_line);
    return os;
}