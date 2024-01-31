#include "token.hpp"

#include <format>
#include <iostream>

using namespace cppLox::Frontend;

Token::Token(Token::Type type, std::string lexeme, std::size_t line) : m_type(type), m_lexeme(lexeme), m_line(line) {
}
