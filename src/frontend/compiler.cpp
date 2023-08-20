#include "compiler.hpp"

#include <format>
#include <iostream>

#include "../bytecode/opcode.hpp"
#include "../error/compiletime_exception.hpp"
#include "../types/object_string.hpp"
#include "lexer.hpp"

using namespace cppLox::Frontend;

Compiler::Compiler(MemoryMutator * memoryMutator) {
    m_memoryMutator = memoryMutator;
}

Compiler::~Compiler() {
}

auto Compiler::compile(std::vector<Token> const & tokens) -> std::unique_ptr<cppLox::ByteCode::Chunk> {
    m_previous = nullptr;
    m_current = nullptr;
    m_chunk = new cppLox::ByteCode::Chunk();
    m_currentTokenIndex = 0;
    advance(tokens);
    expression(tokens);
    consume(Token::Type::END_OF_FILE, "Expect end of expression", tokens);
    endCompilation();
    return std::unique_ptr<cppLox::ByteCode::Chunk>(m_chunk);
}

auto Compiler::error(std::string & message) -> void {
    throw cppLox::Error::CompileTimeException(std::format("{} at line {}", message, m_previous->line()));
}

auto Compiler::errorAtCurrent(std::string & message) -> void {
    throw cppLox::Error::CompileTimeException(std::format("{} at line {}", message, m_current->line()));
}

auto Compiler::advance(std::vector<Token> const & tokens) -> void {
    if (m_current && m_current->type() == Token::Type::END_OF_FILE) {
        return;
    }
    m_previous = m_current;
    m_current = &tokens[m_currentTokenIndex];
    m_currentTokenIndex++;
}

auto Compiler::consume(Token::Type type, std::string message, std::vector<Token> const & tokens) -> void {
    if (m_current->type() == type) {
        advance(tokens);
        return;
    }
    errorAtCurrent(message);
}

auto Compiler::printStatement(std::vector<Token> const & tokens) -> void {
    expression(tokens);
    consume(Token::Type::SEMICOLON, "Expect ';' after value", tokens);
    emitByte(cppLox::ByteCode::Opcode::PRINT);
}

auto inline Compiler::emitByte(uint8_t byte) -> void {
    m_chunk->write(byte, m_previous->line());
}

auto Compiler::emitBytes(uint8_t byte1, uint8_t byte2) -> void {
    emitByte(byte1);
    emitByte(byte2);
}

auto Compiler::emitBytes(cppLox::ByteCode::Opcode opcode, uint8_t byte) -> void {
    emitBytes(static_cast<uint8_t>(opcode), byte);
}

auto inline Compiler::emitByte(cppLox::ByteCode::Opcode opcode) -> void {
    emitByte(static_cast<uint8_t>(opcode));
}

auto Compiler::emitBytes(cppLox::ByteCode::Opcode opcode1, cppLox::ByteCode::Opcode opcode2) -> void {
    emitBytes(static_cast<uint8_t>(opcode1), static_cast<uint8_t>(opcode2));
}

auto inline Compiler::emitConstant(cppLox::Types::Value value) -> void {
    emitBytes(cppLox::ByteCode::Opcode::CONSTANT, m_chunk->addConstant(value));
}

auto Compiler::currentChunk() const -> cppLox::ByteCode::Chunk * {
    return m_chunk;
}

auto Compiler::literal(std::vector<Token> const & tokens) -> void {
    switch (m_previous->type()) {
    case Token::Type::FALSE:
        emitByte(cppLox::ByteCode::Opcode::FALSE);
        break;
    case Token::Type::TRUE:
        emitByte(cppLox::ByteCode::Opcode::TRUE);
        break;
    case Token::Type::NULL_:
        emitByte(cppLox::ByteCode::Opcode::NULL_);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::endCompilation() -> void {
    m_chunk->write(cppLox::ByteCode::Opcode::RETURN, m_previous->line());
}

auto Compiler::expression(std::vector<Token> const & tokens) -> void {
    parsePrecedence(Precedence::ASSIGNMENT, tokens);
}

auto Compiler::number(std::vector<Token> const & tokens) -> void {
    double value = std::stod(m_previous->lexeme());
    emitConstant(value);
}

auto Compiler::makeConstant(cppLox::Types::Value value) -> void {
    int constant = m_chunk->addConstant(value);
    if (constant > UINT8_MAX) {
        throw cppLox::Error::CompileTimeException("Too many constants in one chunk");
    }
    emitBytes(static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT), (uint8_t)constant);
}

auto Compiler::grouping(std::vector<Token> const & tokens) -> void {
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after expression", tokens);
}

auto Compiler::unary(std::vector<Token> const & tokens) -> void {
    Token::Type operatorType = m_previous->type();
    parsePrecedence(Precedence::UNARY, tokens);
    switch (operatorType) {
    case Token::Type::BANG:
        emitByte(cppLox::ByteCode::Opcode::NOT);
        break;
    case Token::Type::MINUS:
        emitByte(cppLox::ByteCode::Opcode::NEGATE);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::string(std::vector<Token> const & tokens) -> void {
    emitConstant(cppLox::Types::Value(m_memoryMutator->create<cppLox::Types::ObjectString>(m_previous->lexeme())));
}

auto Compiler::getRule(Token::Type type) -> ParseRule * {
    return &(m_rules[static_cast<size_t>(type)]);
}

auto Compiler::binary(std::vector<Token> const & tokens) -> void {
    Token::Type operatorType = m_previous->type();
    // Compile the right operand.
    auto rule = getRule(operatorType);
    parsePrecedence((Precedence)((int)rule->precedence() + 1), tokens);
    // Emit the operator instruction.
    switch (operatorType) {
    case Token::Type::BANG_EQUAL:
        emitByte(cppLox::ByteCode::Opcode::NOT_EQUAL);
        break;
    case Token::Type::EQUAL_EQUAL:
        emitByte(cppLox::ByteCode::Opcode::EQUAL);
        break;
    case Token::Type::GREATER:
        emitByte(cppLox::ByteCode::Opcode::GREATER);
        break;
    case Token::Type::GREATER_EQUAL:
        emitByte(cppLox::ByteCode::Opcode::GREATER_EQUAL);
        break;
    case Token::Type::LESS:
        emitByte(cppLox::ByteCode::Opcode::LESS);
        break;
    case Token::Type::LESS_EQUAL:
        emitByte(cppLox::ByteCode::Opcode::LESS_EQUAL);
        break;
    case Token::Type::MINUS:
        emitByte(cppLox::ByteCode::Opcode::SUBTRACT);
        break;
    case Token::Type::PLUS:
        emitByte(cppLox::ByteCode::Opcode::ADD);
        break;
    case Token::Type::STAR:
        emitByte(cppLox::ByteCode::Opcode::MULTIPLY);
        break;
    case Token::Type::SLASH:
        emitByte(cppLox::ByteCode::Opcode::DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void {
    advance(tokens);
    auto prefixRule = getRule(m_previous->type())->prefix();
    if (!prefixRule.has_value()) {
        throw cppLox::Error::CompileTimeException("Expect expression");
        return;
    }
    (this->*prefixRule.value())(tokens);
    while (precedence <= getRule(m_current->type())->precedence()) {
        advance(tokens);
        auto infixRule = getRule(m_previous->type())->infix();
        if (!infixRule.has_value()) {
            throw cppLox::Error::CompileTimeException("Expect expression");
            return;
        }
        (this->*infixRule.value())(tokens);
    }
}