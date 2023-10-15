#include "lexer.hpp"

#include <format>
#include <iostream>

using namespace cppLox::Frontend;

[[nodiscard]] auto Lexer::tokenize(std::string const & source) -> std::vector<Token> {
    // Reset the lexer
    this->m_start = 0;
    this->m_current = 0;
    this->m_line = 1;
    this->m_tokens.clear();
    while (!isAtEnd(source)) {
        skipWhitespace(source);
        m_start = m_current;
        scanToken(source);
    }
    m_tokens.push_back(Token(Token::Type::END_OF_FILE, "", m_line));
    return m_tokens;
}

auto Lexer::scanToken(std::string const & source) -> void {
    char c = advance(source);
    switch (c) {
    case '(':
        addToken(Token::Type::LEFT_PARENTHESES, source);
        break;
    case ')':
        addToken(Token::Type::RIGHT_PARENTHESES, source);
        break;
    case '{':
        addToken(Token::Type::LEFT_BRACE, source);
        break;
    case '}':
        addToken(Token::Type::RIGHT_BRACE, source);
        break;
    case ',':
        addToken(Token::Type::COMMA, source);
        break;
    case '.':
        addToken(Token::Type::DOT, source);
        break;
    case '-':
        addToken(Token::Type::MINUS, source);
        break;
    case '+':
        addToken(Token::Type::PLUS, source);
        break;
    case ';':
        addToken(Token::Type::SEMICOLON, source);
        break;
    case '*':
        addToken(Token::Type::STAR, source);
        break;
    case '!':
        addToken(match('=', source) ? Token::Type::BANG_EQUAL : Token::Type::BANG, source);
        break;
    case '=':
        addToken(match('=', source) ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL, source);
        break;
    case '<':
        addToken(match('=', source) ? Token::Type::LESS_EQUAL : Token::Type::LESS, source);
        break;
    case '>':
        addToken(match('=', source) ? Token::Type::GREATER_EQUAL : Token::Type::GREATER, source);
        break;
    case '/':
        if (match('/', source)) {
            while (peek(source) != '\n' && !isAtEnd(source)) {
                advance(source);
            }
        } else {
            addToken(Token::Type::SLASH, source);
        }
        break;
    case '\n':
        m_line++;
        break;
    case '"':
        string(source);
        break;
    default:
        if (isDigit(c)) {
            number(source);
        } else if (isAlpha(c)) {
            identifier(source);
        } else {
            std::cout << std::format("Unexpected character: {}", c) << std::endl;
        }
        break;
    }
}

auto Lexer::skipWhitespace(std::string const & source) -> void {
    while (!isAtEnd(source) &&
           (peek(source) == ' ' || peek(source) == '\r' || peek(source) == '\t' || peek(source) == '\n')) {
        advance(source);
    }
}

[[nodiscard]] auto Lexer::isAtEnd(std::string const & source) const -> bool {
    return m_current >= source.length();
}

auto Lexer::advance(std::string const & source) -> char {
    return source[m_current++];
}

[[nodiscard]] auto Lexer::match(char expected, std::string const & source) -> bool {
    if (isAtEnd(source)) {
        return false;
    }
    if (source[m_current] != expected) {
        return false;
    }
    m_current++;
    return true;
}

auto Lexer::addToken(Token::Type type, std::string const & lexeme) -> void {
    m_tokens.push_back(Token(type, lexeme, m_line));
}

[[nodiscard]] auto Lexer::peek(std::string const & source) const -> char {
    return source[m_current];
}

[[nodiscard]] auto Lexer::peekNext(std::string const & source) const -> char {
    if (isAtEnd(source)) {
        return '\0';
    }
    return source[m_current + 1];
}

auto Lexer::string(std::string const & source) -> void {
    while (peek(source) != '"' && !isAtEnd(source)) {
        if (peek(source) == '\n') {
            m_line++;
        }
        advance(source);
    }
    if (isAtEnd(source)) {
        std::cout << "Unterminated string." << std::endl;
        return;
    }
    advance(source);
    std::string value = source.substr(m_start + 1, m_current - m_start - 2);
    addToken(Token::Type::STRING, value);
}

[[nodiscard]] auto Lexer::isAlpha(char c) const -> bool {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

[[nodiscard]] auto Lexer::isDigit(char c) const -> bool {
    return c >= '0' && c <= '9';
}

auto Lexer::number(std::string const & source) -> void {
    while (isDigit(peek(source))) {
        advance(source);
    }
    if (peek(source) == '.' && isDigit(peekNext(source))) {
        advance(source);
        while (isDigit(peek(source))) {
            advance(source);
        }
    }
    std::string lexeme = source.substr(m_start, m_current - m_start);
    addToken(Token::Type::NUMBER, lexeme);
}

auto Lexer::identifier(std::string const & source) -> void {
    while (isAlpha(peek(source)) || isDigit(peek(source))) {
        advance(source);
    }
    std::string lexeme = source.substr(m_start, m_current - m_start);
    Token::Type type;
    if (lexeme == "and") {
        type = Token::Type::AND;
    } else if (lexeme == "class") {
        type = Token::Type::CLASS;
    } else if (lexeme == "else") {
        type = Token::Type::ELSE;
    } else if (lexeme == "false") {
        type = Token::Type::FALSE;
    } else if (lexeme == "for") {
        type = Token::Type::FOR;
    } else if (lexeme == "fun") {
        type = Token::Type::FUN;
    } else if (lexeme == "if") {
        type = Token::Type::IF;
    } else if (lexeme == "null") {
        type = Token::Type::NULL_;
    } else if (lexeme == "or") {
        type = Token::Type::OR;
    } else if (lexeme == "print") {
        type = Token::Type::PRINT;
    } else if (lexeme == "return") {
        type = Token::Type::RETURN;
    } else if (lexeme == "super") {
        type = Token::Type::SUPER;
    } else if (lexeme == "this") {
        type = Token::Type::THIS;
    } else if (lexeme == "true") {
        type = Token::Type::TRUE;
    } else if (lexeme == "var") {
        type = Token::Type::VAR;
    } else if (lexeme == "while") {
        type = Token::Type::WHILE;
    } else {
        type = Token::Type::IDENTIFIER;
    }
    addToken(type, lexeme);
}