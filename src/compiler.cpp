#include "compiler.hpp"

#include <format>
#include <iostream>

#include "compiletime_exception.hpp"
#include "lexer.hpp"
#include "opcode.hpp"

Compiler::Compiler() {
}

Compiler::~Compiler() {
}

std::unique_ptr<Chunk> Compiler::compile(std::vector<Token> const & tokens) {
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

void Compiler::error(std::string & message) {
    throw CompileTimeException(std::format("{} at line {}", message, m_previous->line()));
}

void Compiler::errorAtCurrent(std::string & message) {
    throw CompileTimeException(std::format("{} at line {}", message, m_current->line()));
}

void Compiler::advance(std::vector<Token> const & tokens) {
    if (m_current && m_current->type() == Token::Type::END_OF_FILE) {
        return;
    }
    m_previous = m_current;
    m_current = &tokens[m_currentTokenIndex];
    m_currentTokenIndex++;
}

void Compiler::consume(Token::Type type, std::string message, std::vector<Token> const & tokens) {
    if (m_current->type() == type) {
        advance(tokens);
        return;
    }
    errorAtCurrent(message);
}

void Compiler::printStatement(std::vector<Token> const & tokens) {
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

Chunk * Compiler::currentChunk() const {
    return m_chunk;
}

void Compiler::literal(std::vector<Token> const & tokens) {
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

void Compiler::expression(std::vector<Token> const & tokens) {
    parsePrecedence(Precedence::ASSIGNMENT, tokens);
}

void Compiler::number(std::vector<Token> const & tokens) {
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

void Compiler::grouping(std::vector<Token> const & tokens) {
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after expression", tokens);
}

void Compiler::unary(std::vector<Token> const & tokens) {
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

ParseRule const * Compiler::getRule(Token::Type type) {
    return &(m_rules[type]);
}

void Compiler::binary(std::vector<Token> const & tokens) {
    Token::Type operatorType = m_previous->type();
    // Compile the right operand.
    auto rule = getRule(operatorType);
    parsePrecedence((Precedence)((int)rule->m_precedence + 1), tokens);
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

void Compiler::parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) {
    advance(tokens);
    auto prefixRule = getRule(m_previous->type())->prefix();
    if (prefixRule == std::nullopt) {
        throw CompileTimeException("Expect expression");
        return;
    }
    (this->*prefixRule.value())(tokens);
    while (precedence <= getRule(m_current->type())->m_precedence) {
        advance(tokens);
        auto infixRule = getRule(m_previous->type())->infix();
        if (infixRule == std::nullopt) {
            throw CompileTimeException("Expect expression");
            return;
        }
        (this->*infixRule.value())(tokens);
    }
}