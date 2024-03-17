/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file compiler.cpp
 * @brief This file contains the implementation of the Compiler class.
 */

#include "compiler.hpp"

#include <format>
#include <functional>
#include <iostream>
#include <ranges>

#include "../bytecode/opcode.hpp"
#include "../types/object_string.hpp"
#include "lexer.hpp"

using namespace cppLox::Frontend;

Compiler::Compiler(std::shared_ptr<cppLox::MemoryMutator> memoryMutator) : m_memoryMutator(memoryMutator) {
}

auto Compiler::advance(std::vector<Token> const & tokens) -> void {
    if (m_current && m_current->type() == Token::Type::END_OF_FILE) {
        return;
    }
    m_previous = m_current;
    m_current = &tokens[m_currentTokenIndex];
    m_currentTokenIndex++;
}

auto Compiler::and_(std::vector<Token> const & tokens) -> void {
    int endJump = emitJump(cppLox::ByteCode::Opcode::JUMP_IF_FALSE);
    emitByte(cppLox::ByteCode::Opcode::POP);
    parsePrecedence(Precedence::AND, tokens);
    patchJump(endJump);
}

auto Compiler::beginScope() -> void {
    m_currentScope->beginNewScope();
}

auto Compiler::binary(std::vector<Token> const & tokens) -> void {
    Token::Type operatorType = m_previous->type();
    // Compile the right operand.
    ParseRule<cppLox::Frontend::Compiler> * rule = getRule(operatorType);
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

auto Compiler::block(std::vector<Token> const & tokens) -> void {
    while (!check(Token::Type::RIGHT_BRACE) && !check(Token::Type::END_OF_FILE)) {
        declaration(tokens);
    }
    consume(Token::Type::RIGHT_BRACE, "Expect '}' after block", tokens);
}

auto Compiler::check(Token::Type type) const -> bool {
    return m_current->type() == type;
}

auto Compiler::compile(std::vector<Token> const & tokens) -> std::optional<cppLox::Types::ObjectFunction *> {
    m_previous = nullptr;
    m_current = nullptr;
    m_currentTokenIndex = 0;
    m_panicMode = false;
    m_hadError = false;
    initCompiler(FunctionType::SCRIPT);
    advance(tokens);
    while (m_current->type() != Token::Type::END_OF_FILE) {
        declaration(tokens);
    }
    consume(Token::Type::END_OF_FILE, "Expect end of expression", tokens);
    return m_hadError == true ? std::nullopt : std::optional<cppLox::Types::ObjectFunction *>(endCompilation());
}

auto Compiler::consume(Token::Type type, std::string message, std::vector<Token> const & tokens) -> void {
    if (m_current->type() == type) {
        advance(tokens);
        return;
    }
    errorAtCurrent(message);
}

auto Compiler::currentChunk() const -> cppLox::ByteCode::Chunk * {
    return m_currentScope->function()->chunk();
}

auto Compiler::declaration(std::vector<Token> const & tokens) -> void {
    if (match(Token::Type::FUN, tokens)) {
        funDeclaration(tokens);
    } else if (match(Token::Type::VAR, tokens)) {
        variableDeclaration(tokens);
    } else {
        statement(tokens);
    }

    if (m_panicMode) {
        synchronize(tokens);
    }
}

auto Compiler::declareVariable(std::vector<Token> const & tokens) -> void {
    // Global variables are implicitly declared.
    if (m_currentScope->scopeDepth() == 0) {
        return;
    }
    std::string const & name = m_previous->lexeme();
    for (auto i : std::views::iota(0u, m_currentScope->localScope()->localCount()) | std::views::reverse) {
        auto local = m_currentScope->localScope()->local(i);
        if (local.getDepth() != -1 && local.getDepth() < m_currentScope->scopeDepth()) {
            break;
        }
        if (name == local.getToken().lexeme()) {
            error("Variable with this name already declared in this scope");
        }
    }
    m_currentScope->localScope()->addLocal(*m_previous, [&](std::string & message) { error(message); });
}

auto Compiler::defineVariable(uint8_t global) -> void {
    if (m_currentScope->scopeDepth() > 0) {
        m_currentScope->localScope()->markInitialized(m_currentScope->scopeDepth());
        return;
    }
    emitBytes(cppLox::ByteCode::Opcode::DEFINE_GLOBAL, global);
}

auto inline Compiler::emitByte(uint8_t byte) -> void {
    currentChunk()->write(byte, m_previous->line());
}

auto inline Compiler::emitByte(cppLox::ByteCode::Opcode opcode) -> void {
    emitByte(static_cast<uint8_t>(opcode));
}

auto inline Compiler::emitConstant(cppLox::Types::Value value) -> void {
    emitBytes(cppLox::ByteCode::Opcode::CONSTANT, (uint8_t)currentChunk()->addConstant(value));
}

auto inline Compiler::emitJump(cppLox::ByteCode::Opcode opcode) -> int32_t {
    emitByte(opcode);
    emitByte(0xff);
    emitByte(0xff);
    return currentChunk()->getSize() - 2;
}

auto inline Compiler::emitLoop(int32_t loopStart) -> void {
    emitByte(cppLox::ByteCode::Opcode::LOOP);
    int32_t offset = currentChunk()->getSize() - loopStart + 2;
    if (offset > UINT16_MAX) {
        error("Loop body too large");
    }
    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

auto inline Compiler::emitReturn() -> void {
    emitByte(cppLox::ByteCode::Opcode::RETURN);
}

auto Compiler::endScope() -> void {
    if (m_currentScope->scopeDepth() == 0) {
        error("No enclosing scope found for endScope() call");
    }
    while (m_currentScope->localScope()->localCount() > 0 &&
           m_currentScope->localScope()->local(m_currentScope->localScope()->localCount() - 1).getDepth() >=
               m_currentScope->scopeDepth()) {
        emitByte(cppLox::ByteCode::Opcode::POP);
        m_currentScope->localScope()->popLocal();
    }
    m_currentScope->endScope();
}

auto Compiler::endCompilation() -> cppLox::Types::ObjectFunction * {
    emitReturn();
    cppLox::Types::ObjectFunction * function = m_currentScope->function();
#ifdef DEBUG_PRINT_CODE
    function->chunk()->disassemble(function->name() != nullptr ? function->name()->string() : "<script>");
#endif
    if (m_currentScope->enclosing().has_value()) {
        m_currentScope = m_currentScope->enclosing().value();
    }
    return function;
}

auto Compiler::error(char const * message) -> void {
    m_hadError = true;
    m_panicMode = true;
    std::cerr << std::format("{} at line {}", message, m_previous->line()) << std::endl;
}

auto Compiler::error(std::string & message) -> void {
    m_hadError = true;
    m_panicMode = true;
    std::cerr << std::format("{} at line {}", message, m_previous->line()) << std::endl;
}

auto Compiler::errorAtCurrent(std::string & message) -> void {
    m_hadError = true;
    m_panicMode = true;
    std::cerr << std::format("{} at line {}", message, m_current->line()) << std::endl;
}

auto Compiler::expression(std::vector<Token> const & tokens) -> void {
    parsePrecedence(Precedence::ASSIGNMENT, tokens);
}

auto Compiler::expressionStatement(std::vector<Token> const & tokens) -> void {
    expression(tokens);
    consume(Token::Type::SEMICOLON, "Expect ';' after expression", tokens);
    emitByte(cppLox::ByteCode::Opcode::POP);
}

auto Compiler::forStatement(std::vector<Token> const & tokens) -> void {
    beginScope();
    consume(Token::Type::LEFT_PARENTHESES, "Expect '(' after 'for'", tokens);
    if (match(Token::Type::SEMICOLON, tokens)) {
        // No initializer.
    } else if (match(Token::Type::VAR, tokens)) {
        variableDeclaration(tokens);
    } else {
        expressionStatement(tokens);
    }
    int loopStart = currentChunk()->getSize();
    int exitJump = -1;
    if (!match(Token::Type::SEMICOLON, tokens)) {
        expression(tokens);
        consume(Token::Type::SEMICOLON, "Expect ';' after loop condition", tokens);
        exitJump = emitJump(cppLox::ByteCode::Opcode::JUMP_IF_FALSE);
        emitByte(cppLox::ByteCode::Opcode::POP);
    }
    if (!match(Token::Type::RIGHT_PARENTHESES, tokens)) {
        int bodyJump = emitJump(cppLox::ByteCode::Opcode::JUMP);
        int incrementStart = currentChunk()->getSize();
        expression(tokens);
        emitByte(cppLox::ByteCode::Opcode::POP);
        consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after for clauses", tokens);
        emitLoop(loopStart);
        loopStart = incrementStart;
        patchJump(bodyJump);
    }
    statement(tokens);
    emitLoop(loopStart);
    if (exitJump != -1) {
        patchJump(exitJump);
        emitByte(cppLox::ByteCode::Opcode::POP);
    }
    endScope();
}

auto Compiler::funDeclaration(std::vector<Token> const & tokens) -> void {
    uint8_t global = parseVariable("Expect function name", tokens);
    markInitialized();
    function(FunctionType::FUNCTION, tokens);
    defineVariable(global);
}

auto Compiler::function(FunctionType type, std::vector<Token> const & tokens) -> void {
    initCompiler(type);
    beginScope();
    consume(Token::Type::LEFT_PARENTHESES, "Expect '(' after function name", tokens);
    if (!check(Token::Type::RIGHT_PARENTHESES)) {
        do {
            m_currentScope->function()->incrementArity();
            if (m_currentScope->function()->arity() > 255) {
                error("Cannot have more than 255 parameters");
            }
            uint8_t constant = parseVariable("Expect parameter name", tokens);
            defineVariable(constant);
        } while (match(Token::Type::COMMA, tokens));
    }
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after parameters", tokens);
    consume(Token::Type::LEFT_BRACE, "Expect '{' before function body", tokens);
    block(tokens);
    auto function = endCompilation();
    emitConstant(cppLox::Types::Value(static_cast<cppLox::Types::Object *>(function)));
}

auto Compiler::getRule(Token::Type type) -> ParseRule<Compiler> * {
    return &(m_rules[static_cast<size_t>(type)]);
}

auto Compiler::identifierConstant(Token const & name) -> uint8_t {
    return currentChunk()->addConstant(
        cppLox::Types::Value(m_memoryMutator->create<cppLox::Types::ObjectString>(name.lexeme())));
}

auto Compiler::ifStatement(std::vector<Token> const & tokens) -> void {
    consume(Token::Type::LEFT_PARENTHESES, "Expect '(' after 'if'", tokens);
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after condition", tokens);
    int thenJump = emitJump(cppLox::ByteCode::Opcode::JUMP_IF_FALSE);
    emitByte(cppLox::ByteCode::Opcode::POP);
    if (m_current->type() != Token::Type::END_OF_FILE) {
        statement(tokens);
    }
    patchJump(thenJump);
    emitByte(cppLox::ByteCode::Opcode::POP);
    if (match(Token::Type::ELSE, tokens)) {
        int elseJump = emitJump(cppLox::ByteCode::Opcode::JUMP);
        patchJump(thenJump);
        if (m_current->type() != Token::Type::END_OF_FILE) {
            statement(tokens);
        }
        patchJump(elseJump);
    }
}

auto Compiler::initCompiler(FunctionType type) -> void {
    auto name = type == FunctionType::SCRIPT
                    ? m_memoryMutator->create<cppLox::Types::ObjectString>("main")->as<cppLox::Types::ObjectString>()
                    : m_memoryMutator->create<cppLox::Types::ObjectString>(m_previous->lexeme())
                          ->as<cppLox::Types::ObjectString>();
    auto function =
        m_memoryMutator->create<cppLox::Types::ObjectFunction>(0, name)->as<cppLox::Types::ObjectFunction>();
    m_currentScope = std::make_shared<CompilationScope>(m_currentScope, function, type);
    m_currentScope->localScope()->addLocal(Token(Token::Type::IDENTIFIER, "", 0),
                                           [&](std::string & message) { error(message); });
}

auto Compiler::literal(std::vector<Token> const & tokens, bool canAssign) -> void {
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

auto Compiler::grouping(std::vector<Token> const & tokens, bool canAssign) -> void {
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after expression", tokens);
}

auto Compiler::makeConstant(cppLox::Types::Value value) -> void {
    auto constant = currentChunk()->addConstant(value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk");
    }
    emitBytes(static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT), (uint8_t)constant);
}

auto Compiler::markInitialized() -> void {
    if (m_currentScope->scopeDepth() == 0) {
        return;
    }
    m_currentScope->localScope()->markInitialized(m_currentScope->scopeDepth());
}

auto Compiler::match(Token::Type type, std::vector<Token> const & tokens) -> bool {
    if (m_current->type() != type) {
        return false;
    }
    advance(tokens);
    return true;
}

auto Compiler::namedVariable(Token const & name, std::vector<Token> const & tokens, bool canAssign) -> void {
    uint8_t getOp, setOp;
    int32_t arg = resolveLocal(name, *m_currentScope->localScope().get());
    if (arg != -1) {
        getOp = cppLox::ByteCode::Opcode::GET_LOCAL;
        setOp = cppLox::ByteCode::Opcode::SET_LOCAL;
    } else {
        arg = identifierConstant(name);
        getOp = cppLox::ByteCode::Opcode::GET_GLOBAL;
        setOp = cppLox::ByteCode::Opcode::SET_GLOBAL;
    }
    if (canAssign && match(Token::Type::EQUAL, tokens)) {
        expression(tokens);
        emitBytes(setOp, (uint8_t)arg);
    } else {
        emitBytes(getOp, (uint8_t)arg);
    }
}

auto Compiler::number(std::vector<Token> const & tokens, bool canAssign) -> void {
    double value = std::stod(m_previous->lexeme());
    emitConstant(value);
}

auto Compiler::or_(std::vector<Token> const & tokens) -> void {
    int elseJump = emitJump(cppLox::ByteCode::Opcode::JUMP_IF_FALSE);
    int endJump = emitJump(cppLox::ByteCode::Opcode::JUMP);
    patchJump(elseJump);
    emitByte(cppLox::ByteCode::Opcode::POP);
    parsePrecedence(Precedence::OR, tokens);
    patchJump(endJump);
}

auto Compiler::parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void {
    advance(tokens);
    auto canAssign = precedence <= Precedence::ASSIGNMENT;
    auto prefixRule = getRule(m_previous->type())->prefix();
    if (!prefixRule.has_value()) {
        error("Expect expression");
        return;
    }
    (this->*prefixRule.value())(tokens, canAssign);
    while (precedence <= getRule(m_current->type())->precedence()) {
        advance(tokens);
        auto infixRule = getRule(m_previous->type())->infix();
        if (!infixRule.has_value()) {
            error("Expect expression");
            return;
        }
        (this->*infixRule.value())(tokens);
    }
    if (canAssign && match(Token::Type::EQUAL, tokens)) {
        error("Invalid assignment target");
    }
}

auto Compiler::parseVariable(std::string message, std::vector<Token> const & tokens) -> uint8_t {
    consume(Token::Type::IDENTIFIER, message, tokens);
    declareVariable(tokens);
    if (m_currentScope->scopeDepth() > 0) {
        return 0;
    }
    return identifierConstant(*m_previous);
}

auto Compiler::patchJump(int32_t offset) -> void {
    // -2 to adjust for the bytecode for the jump offset itself.
    int32_t jump = currentChunk()->getSize() - offset - 2;
    if (jump > UINT16_MAX) {
        error("Too much code to jump over");
    }
    currentChunk()->writeAt(offset, (jump >> 8) & 0xff);
    currentChunk()->writeAt(offset + 1, jump & 0xff);
}

auto Compiler::printStatement(std::vector<Token> const & tokens, bool canAssign) -> void {
    advance(tokens);
    expression(tokens);
    consume(Token::Type::SEMICOLON, "Expect ';' after value", tokens);
    emitByte(cppLox::ByteCode::Opcode::PRINT);
}

auto Compiler::statement(std::vector<Token> const & tokens) -> void {
    if (m_current->type() == Token::Type::PRINT) {
        printStatement(tokens, false);
    } else if (match(Token::Type::FOR, tokens)) {
        forStatement(tokens);
    } else if (match(Token::Type::IF, tokens)) {
        ifStatement(tokens);
    } else if (match(Token::Type::WHILE, tokens)) {
        whileStatement(tokens);
    } else if (match(Token::Type::LEFT_BRACE, tokens)) {
        beginScope();
        block(tokens);
        endScope();
    } else {
        expressionStatement(tokens);
    }
}

auto Compiler::resolveLocal(Token const & name, LocalScope const & scope) -> int {
    for (auto i : std::views::iota(0u, scope.localCount()) | std::views::reverse) {
        auto local = scope.local(i);
        if (name.lexeme() == local.getToken().lexeme()) {
            if (local.getDepth() == -1) {
                error("Cannot read local variable in its own initializer");
            }
            return i;
        }
    }
    // If the local wasn't found in the current scope, check the enclosing scope.
    if (scope.enclosing().has_value()) {
        return resolveLocal(name, *scope.enclosing().value());
    }
    return -1;
}

auto Compiler::string(std::vector<Token> const & tokens, bool canAssign) -> void {
    emitConstant(cppLox::Types::Value(m_memoryMutator->create<cppLox::Types::ObjectString>(m_previous->lexeme())));
}

auto Compiler::synchronize(std::vector<Token> const & tokens) -> void {
    m_panicMode = false;
    advance(tokens);
    while (m_current->type() != Token::Type::END_OF_FILE) {
        if (m_previous->type() == Token::Type::SEMICOLON) {
            return;
        }
        switch (m_current->type()) {
        case Token::Type::CLASS:
        case Token::Type::FUN:
        case Token::Type::VAR:
        case Token::Type::FOR:
        case Token::Type::IF:
        case Token::Type::WHILE:
        case Token::Type::PRINT:
        case Token::Type::RETURN:
            return;
        default:
            // Do nothing.
            break;
        }
        advance(tokens);
    }
}

auto Compiler::variable(std::vector<Token> const & tokens, bool canAssign) -> void {
    namedVariable(*m_previous, tokens, canAssign);
}

auto Compiler::variableDeclaration(std::vector<Token> const & tokens) -> void {
    uint8_t global = parseVariable("Expect variable name", tokens);
    if (match(Token::Type::EQUAL, tokens)) {
        expression(tokens);
    } else {
        emitByte(cppLox::ByteCode::Opcode::NULL_);
    }
    consume(Token::Type::SEMICOLON, "Expect ';' after variable declaration", tokens);
    defineVariable(global);
}

auto Compiler::unary(std::vector<Token> const & tokens, bool canAssign) -> void {
    auto operatorType = m_previous->type();
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

auto Compiler::whileStatement(std::vector<Token> const & tokens) -> void {
    int32_t loopStart = currentChunk()->getSize();
    consume(Token::Type::LEFT_PARENTHESES, "Expect '(' after 'while'", tokens);
    expression(tokens);
    consume(Token::Type::RIGHT_PARENTHESES, "Expect ')' after condition", tokens);
    int32_t exitJump = emitJump(cppLox::ByteCode::Opcode::JUMP_IF_FALSE);
    emitByte(cppLox::ByteCode::Opcode::POP);
    statement(tokens);
    emitLoop(loopStart);
    patchJump(exitJump);
    emitByte(cppLox::ByteCode::Opcode::POP);
}