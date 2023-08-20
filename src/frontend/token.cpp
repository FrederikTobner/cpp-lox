#include "token.hpp"

#include <format>
#include <iostream>

using namespace cppLox::Frontend;

Token::Token(Type type, std::string lexeme, size_t line) {
    this->m_type = type;
    this->m_lexeme = lexeme;
    this->m_line = line;
}