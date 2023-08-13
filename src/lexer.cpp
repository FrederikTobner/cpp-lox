#include "lexer.hpp"

#include <iostream>

Lexer::Lexer() {
    this->m_tokens = std::vector<Token>();
}

[[nodiscard]] std::vector<Token> Lexer::tokenize(const std::string & source) {
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

void Lexer::scanToken(std::string const & source) {
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
            std::cout << "Unexpected character: " << c << std::endl;
        }
        break;
    }
}

void Lexer::skipWhitespace(std::string const & source) {
    while (peek(source) == ' ' || peek(source) == '\r' || peek(source) == '\t') {
        advance(source);
    }
}

[[nodiscard]] bool Lexer::isAtEnd(std::string const & source) {
    return m_current >= source.length();
}

char Lexer::advance(std::string const & source) {
    return source[m_current++];
}

[[nodiscard]] bool Lexer::match(char expected, std::string const & source) {
    if (isAtEnd(source)) {
        return false;
    }
    if (source[m_current] != expected) {
        return false;
    }
    m_current++;
    return true;
}

void Lexer::addToken(Token::Type type, std::string const & lexeme) {
    m_tokens.push_back(Token(type, lexeme, m_line));
}

[[nodiscard]] char Lexer::peek(std::string const & source) {
    return source[m_current];
}

[[nodiscard]] char Lexer::peekNext(std::string const & source) {
    if (isAtEnd(source)) {
        return '\0';
    }
    return source[m_current + 1];
}

void Lexer::string(std::string const & source) {
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

[[nodiscard]] bool Lexer::isAlpha(char c) const {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

[[nodiscard]] bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

void Lexer::number(std::string const & source) {
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

void Lexer::identifier(std::string const & source) {
    while (isAlpha(peek(source)) || isDigit(peek(source))) {
        advance(source);
    }
    std::string text = source.substr(m_start, m_current - m_start);
    Token::Type type;
    if (text == "and") {
        type = Token::Type::AND;
    } else if (text == "class") {
        type = Token::Type::CLASS;
    } else if (text == "else") {
        type = Token::Type::ELSE;
    } else if (text == "false") {
        type = Token::Type::FALSE;
    } else if (text == "for") {
        type = Token::Type::FOR;
    } else if (text == "fun") {
        type = Token::Type::FUN;
    } else if (text == "if") {
        type = Token::Type::IF;
    } else if (text == "null") {
        type = Token::Type::NULL_;
    } else if (text == "or") {
        type = Token::Type::OR;
    } else if (text == "print") {
        type = Token::Type::PRINT;
    } else if (text == "return") {
        type = Token::Type::RETURN;
    } else if (text == "super") {
        type = Token::Type::SUPER;
    } else if (text == "this") {
        type = Token::Type::THIS;
    } else if (text == "true") {
        type = Token::Type::TRUE;
    } else if (text == "var") {
        type = Token::Type::VAR;
    } else if (text == "while") {
        type = Token::Type::WHILE;
    } else {
        type = Token::Type::IDENTIFIER;
    }
    std::string lexeme = source.substr(m_start, m_current - m_start);
    addToken(type, lexeme);
}