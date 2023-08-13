#include "compiler.hpp"

#include <iostream>

#include "compiletime_exception.hpp"
#include "lexer.hpp"
#include "opcode.hpp"

Compiler::Compiler() {
    m_rules[Token::Type::LEFT_PARENTHESES] = ParseRule(&Compiler::grouping, nullptr, Precedence::NONE);
    m_rules[Token::Type::RIGHT_PARENTHESES] = Compiler::ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::LEFT_BRACE] = Compiler::ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::RIGHT_BRACE] = Compiler::ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::COMMA] = Compiler::ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::DOT] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::MINUS] = ParseRule(&Compiler::unary, &Compiler::binary, Precedence::TERM);
    m_rules[Token::Type::PLUS] = ParseRule(nullptr, &Compiler::binary, Precedence::TERM);
    m_rules[Token::Type::SEMICOLON] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::SLASH] = ParseRule(nullptr, &Compiler::binary, Precedence::FACTOR);
    m_rules[Token::Type::STAR] = ParseRule(nullptr, &Compiler::binary, Precedence::FACTOR);
    m_rules[Token::Type::BANG] = ParseRule(&Compiler::unary, nullptr, Precedence::NONE);
    m_rules[Token::Type::BANG_EQUAL] = ParseRule(nullptr, &Compiler::binary, Precedence::EQUALITY);
    m_rules[Token::Type::EQUAL] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::EQUAL_EQUAL] = ParseRule(nullptr, &Compiler::binary, Precedence::NONE);
    m_rules[Token::Type::GREATER] = ParseRule(nullptr, &Compiler::binary, Precedence::NONE);
    m_rules[Token::Type::GREATER_EQUAL] = ParseRule(nullptr, &Compiler::binary, Precedence::NONE);
    m_rules[Token::Type::LESS] = ParseRule(nullptr, &Compiler::binary, Precedence::NONE);
    m_rules[Token::Type::LESS_EQUAL] = ParseRule(nullptr, &Compiler::binary, Precedence::NONE);
    m_rules[Token::Type::IDENTIFIER] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::STRING] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::NUMBER] = ParseRule(&Compiler::number, nullptr, Precedence::NONE);
    m_rules[Token::Type::AND] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::CLASS] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::ELSE] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::FALSE] = ParseRule(&Compiler::literal, nullptr, Precedence::NONE);
    m_rules[Token::Type::FUN] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::FOR] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::IF] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::NULL_] = ParseRule(&Compiler::literal, nullptr, Precedence::NONE);
    m_rules[Token::Type::OR] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::PRINT] = ParseRule(&Compiler::printStatement, nullptr, Precedence::NONE);
    m_rules[Token::Type::RETURN] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::SUPER] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::THIS] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::TRUE] = ParseRule(&Compiler::literal, nullptr, Precedence::NONE);
    m_rules[Token::Type::VAR] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::WHILE] = ParseRule(nullptr, nullptr, Precedence::NONE);
    m_rules[Token::Type::END_OF_FILE] = ParseRule(nullptr, nullptr, Precedence::NONE);
}

Compiler::~Compiler() {
}

std::unique_ptr<Chunk> Compiler::compile(std::vector<Token> & tokens) {
    m_previous = nullptr;
    m_current = nullptr;
    m_chunk = new Chunk();
    m_currentTokenIndex = 0;
    advance(tokens);
    expression(tokens);
    consume(Token::Type::END_OF_FILE, "Expect end of expression", tokens);
    endCompilation();
    return std::unique_ptr<Chunk>(m_chunk);
}

void Compiler::error(std::string message) {
    throw CompileTimeException(message.append(" at line ").append(std::to_string(m_previous->line())));
}

void Compiler::errorAtCurrent(std::string message) {
    throw CompileTimeException(message.append(" at line ").append(std::to_string(m_current->line())));
}

void Compiler::advance(std::vector<Token> & tokens) {
    if (m_current && m_current->type() == Token::Type::END_OF_FILE) {
        return;
    }
    m_previous = m_current;
    m_current = &tokens[m_currentTokenIndex];
    m_currentTokenIndex++;
}

void Compiler::consume(Token::Type type, std::string message, std::vector<Token> & tokens) {
    if (m_current->type() == type) {
        advance(tokens);
        return;
    }
    errorAtCurrent(message);
}

void Compiler::printStatement(std::vector<Token> & tokens) {
    expression(tokens);
    consume(Token::Type::SEMICOLON, "Expect ';' after value", tokens);
    emitByte(OP_PRINT);
}

void inline Compiler::emitByte(uint8_t byte) {
    m_chunk->write(byte, m_previous->line());
}

void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

void inline Compiler::emitConstant(Value value) {
    emitBytes(OP_CONSTANT, m_chunk->addConstant(value));
}

Chunk * Compiler::currentChunk() {
    return m_chunk;
}

void Compiler::literal(std::vector<Token> & tokens) {
    switch (m_previous->type()) {
    case Token::Type::FALSE:
        emitByte(OP_FALSE);
        break;
    case Token::Type::TRUE:
        emitByte(OP_TRUE);
        break;
    case Token::Type::NULL_:
        emitByte(OP_NULL);
        break;
    default:
        return; // Unreachable.
    }
}

void Compiler::endCompilation() {
    m_chunk->write(OP_RETURN, m_previous->line());
}

void Compiler::expression(std::vector<Token> & tokens) {
    parsePrecedence(Precedence::ASSIGNMENT, tokens);
}

void Compiler::number(std::vector<Token> & tokens) {
    double value = std::stod(m_previous->lexeme());
    emitConstant(value);
}

void Compiler::makeConstant(Value value) {
    int constant = m_chunk->addConstant(value);
    if (constant > UINT8_MAX) {
        throw CompileTimeException("Too many constants in one chunk");
    }
    emitBytes(OP_CONSTANT, (uint8_t)constant);
}

void Compiler::grouping(std::vector<Token> & tokens) {
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after expression", tokens);
}

void Compiler::unary(std::vector<Token> & tokens) {
    Token::Type operatorType = m_previous->type();
    parsePrecedence(Precedence::UNARY, tokens);
    switch (operatorType) {
    case Token::Type::BANG:
        emitByte(OP_NOT);
        break;
    case Token::Type::MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return; // Unreachable.
    }
}

Compiler::ParseRule * Compiler::getRule(Token::Type type) {
    return &m_rules[type];
}

void Compiler::binary(std::vector<Token> & tokens) {
    Token::Type operatorType = m_previous->type();
    // Compile the right operand.
    auto rule = getRule(operatorType);
    parsePrecedence((Precedence)((int)rule->precedence() + 1), tokens);
    // Emit the operator instruction.
    switch (operatorType) {
    case Token::Type::BANG_EQUAL:
        emitByte(OP_NOT_EQUAL);
        break;
    case Token::Type::EQUAL_EQUAL:
        emitByte(OP_EQUAL);
        break;
    case Token::Type::GREATER:
        emitByte(OP_GREATER);
        break;
    case Token::Type::GREATER_EQUAL:
        emitByte(OP_GREATER_EQUAL);
        break;
    case Token::Type::LESS:
        emitByte(OP_LESS);
        break;
    case Token::Type::LESS_EQUAL:
        emitByte(OP_LESS_EQUAL);
        break;
    case Token::Type::MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case Token::Type::PLUS:
        emitByte(OP_ADD);
        break;
    case Token::Type::STAR:
        emitByte(OP_MULTIPLY);
        break;
    case Token::Type::SLASH:
        emitByte(OP_DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}

void Compiler::parsePrecedence(Precedence precedence, std::vector<Token> & tokens) {
    advance(tokens);
    Parse_func prefixRule = getRule(m_previous->type())->prefix();
    if (prefixRule == nullptr) {
        throw CompileTimeException("Expect expression");
        return;
    }
    (this->*prefixRule)(tokens);
    while (precedence <= getRule(m_current->type())->precedence()) {
        advance(tokens);
        Parse_func infixRule = getRule(m_previous->type())->infix();
        (this->*infixRule)(tokens);
    }
}