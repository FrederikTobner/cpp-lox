#include "compiler.hpp"

#include <format>
#include <iostream>

#include "compiletime_exception.hpp"
#include "lexer.hpp"
#include "opcode.hpp"

Compiler::Compiler(MemoryManager * memoryManager) {
    m_memoryManager = memoryManager;
}

Compiler::~Compiler() {
}

auto Compiler::compile(std::vector<Token> const & tokens) -> std::unique_ptr<Chunk> {
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

auto Compiler::error(std::string & message) -> void {
    throw CompileTimeException(std::format("{} at line {}", message, m_previous->line()));
}

auto Compiler::errorAtCurrent(std::string & message) -> void {
    throw CompileTimeException(std::format("{} at line {}", message, m_current->line()));
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
    emitByte(Opcode::PRINT);
}

auto inline Compiler::emitByte(uint8_t byte) -> void {
    m_chunk->write(byte, m_previous->line());
}

auto Compiler::emitBytes(uint8_t byte1, uint8_t byte2) -> void {
    emitByte(byte1);
    emitByte(byte2);
}

auto Compiler::emitBytes(Opcode opcode, uint8_t byte) -> void {
    emitBytes(static_cast<uint8_t>(opcode), byte);
}

auto inline Compiler::emitByte(Opcode opcode) -> void {
    emitByte(static_cast<uint8_t>(opcode));
}

auto Compiler::emitBytes(Opcode opcode1, Opcode opcode2) -> void {
    emitBytes(static_cast<uint8_t>(opcode1), static_cast<uint8_t>(opcode2));
}

auto inline Compiler::emitConstant(Value value) -> void {
    emitBytes(Opcode::CONSTANT, m_chunk->addConstant(value));
}

auto Compiler::currentChunk() const -> Chunk * {
    return m_chunk;
}

auto Compiler::literal(std::vector<Token> const & tokens) -> void {
    switch (m_previous->type()) {
    case Token::Type::FALSE:
        emitByte(Opcode::FALSE);
        break;
    case Token::Type::TRUE:
        emitByte(Opcode::TRUE);
        break;
    case Token::Type::NULL_:
        emitByte(Opcode::NULL_);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::endCompilation() -> void {
    m_chunk->write(Opcode::RETURN, m_previous->line());
}

auto Compiler::expression(std::vector<Token> const & tokens) -> void {
    parsePrecedence(Precedence::ASSIGNMENT, tokens);
}

auto Compiler::number(std::vector<Token> const & tokens) -> void {
    double value = std::stod(m_previous->lexeme());
    emitConstant(value);
}

auto Compiler::makeConstant(Value value) -> void {
    int constant = m_chunk->addConstant(value);
    if (constant > UINT8_MAX) {
        throw CompileTimeException("Too many constants in one chunk");
    }
    emitBytes(static_cast<uint8_t>(Opcode::CONSTANT), (uint8_t)constant);
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
        emitByte(Opcode::NOT);
        break;
    case Token::Type::MINUS:
        emitByte(Opcode::NEGATE);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::string(std::vector<Token> const & tokens) -> void {
    emitConstant(Value(m_memoryManager->create<ObjectString>(m_previous->lexeme())));
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
        emitByte(Opcode::NOT_EQUAL);
        break;
    case Token::Type::EQUAL_EQUAL:
        emitByte(Opcode::EQUAL);
        break;
    case Token::Type::GREATER:
        emitByte(Opcode::GREATER);
        break;
    case Token::Type::GREATER_EQUAL:
        emitByte(Opcode::GREATER_EQUAL);
        break;
    case Token::Type::LESS:
        emitByte(Opcode::LESS);
        break;
    case Token::Type::LESS_EQUAL:
        emitByte(Opcode::LESS_EQUAL);
        break;
    case Token::Type::MINUS:
        emitByte(Opcode::SUBTRACT);
        break;
    case Token::Type::PLUS:
        emitByte(Opcode::ADD);
        break;
    case Token::Type::STAR:
        emitByte(Opcode::MULTIPLY);
        break;
    case Token::Type::SLASH:
        emitByte(Opcode::DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}

auto Compiler::parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void {
    advance(tokens);
    auto prefixRule = getRule(m_previous->type())->prefix();
    if (!prefixRule.has_value()) {
        throw CompileTimeException("Expect expression");
        return;
    }
    (this->*prefixRule.value())(tokens);
    while (precedence <= getRule(m_current->type())->precedence()) {
        advance(tokens);
        auto infixRule = getRule(m_previous->type())->infix();
        if (!infixRule.has_value()) {
            throw CompileTimeException("Expect expression");
            return;
        }
        (this->*infixRule.value())(tokens);
    }
}