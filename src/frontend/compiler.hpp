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
 * @file compiler.hpp
 * @brief This file contains the declaration of the Compiler class.
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../bytecode/chunk.hpp"
#include "../memory_mutator.hpp"
#include "../types/object_function.hpp"
#include "compilation_scope.hpp"
#include "function_type.hpp"
#include "parse_rule.hpp"
#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

template <typename T1, typename T2, class... Args>
concept IsPackOfEitherOf = (... && (std::is_same_v<Args, T1> || std::is_same_v<Args, T2>));

/// @brief The compiler used by the cpplox interpreter.
class Compiler {

  public:
    /// @brief Constructs a new compiler.
    Compiler(std::shared_ptr<cppLox::MemoryMutator> memoryMutator);

    /// @brief Destructor of the compiler.
    ~Compiler() = default;

    /// @brief Compiles the given tokens.
    /// @param tokens The tokens that are compiled.
    /// @return The compiled chunk.
    [[nodiscard]] auto compile(std::vector<Token> const & tokens) -> std::optional<cppLox::Types::ObjectFunction *>;

  private:
    /// @brief Advances to the next token.
    /// @param tokens The tokens that are compiled.
    auto advance(std::vector<Token> const & tokens) -> void;

    auto and_(std::vector<Token> const & tokens) -> void;

    auto argumentList(std::vector<Token> const & tokens) -> uint8_t;

    /// @brief Begins a new scope.
    auto beginScope() -> void;

    /// @brief Compiles a block.
    /// @param tokens The tokens that are compiled.
    auto block(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a binary expression.
    /// @param tokens The tokens that are compiled.
    auto binary(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a call expression.
    /// @param tokens The tokens that are compiled.
    auto call(std::vector<Token> const & tokens) -> void;

    /// @brief Checks whether the current token is of the given type.
    /// @param type The given type of the token.
    /// @return Whether the current token is of the given type.
    [[nodiscard]] auto inline check(Token::Type type) const -> bool;

    /// @brief Consumes a token.
    /// @param type The type of the token.
    /// @param message The message to display if the token is not of the given type.
    /// @param tokens The tokens that are compiled.
    auto consume(Token::Type type, std::string message, std::vector<Token> const & tokens) -> void;

    /// @brief Gets the current chunk.
    /// @return The current chunk.
    [[nodiscard]] auto currentChunk() const -> cppLox::ByteCode::Chunk *;

    /// @brief Compiles a declaration.
    /// @param tokens The tokens that are compiled.
    auto declaration(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a function declaration.
    /// @param tokens The tokens that are compiled.
    auto declareVariable(std::vector<Token> const & tokens) -> void;

    /// @brief Emits a DEFINE_GLOBAL opcode and the index of the variable in the chunk.
    /// @param global The index of the variable in the chunk.
    auto defineVariable(uint8_t global) -> void;

    /// @brief Emits a byte.
    /// @param byte The byte to emit.
    auto inline emitByte(uint8_t byte) -> void;

    /// @brief Emits several bytes.
    /// @param opcodes The bytes to emit.
    template <class... Opcodes>
        requires IsPackOfEitherOf<uint8_t, cppLox::ByteCode::Opcode, Opcodes...>
    auto emitBytes(Opcodes... opcodes) -> void {
        std::initializer_list<std::variant<uint8_t, cppLox::ByteCode::Opcode>> bytes = {opcodes...};
        for (auto byte : bytes) {
            std::visit([this](auto && arg) { this->emitByte(arg); }, byte);
        }
    }

    /// @brief Emits a byte.
    /// @param byte The byte to emit.
    auto inline emitByte(cppLox::ByteCode::Opcode byte) -> void;

    /// @brief Emits a constant.
    /// @param value The value to emit.
    auto inline emitConstant(cppLox::Types::Value value) -> void;

    /// @brief Emits a jump instruction.
    /// @param opcode The opcode of the jump instruction.
    /// @return The index of the jump instruction in the chunk.
    auto inline emitJump(cppLox::ByteCode::Opcode opcode) -> int32_t;

    /// @brief Emits a loop instruction.
    /// @param loopStart The index of the loop start.
    auto inline emitLoop(int32_t loopStart) -> void;

    auto inline emitReturn() -> void;

    /// @brief Ends the current scope.
    auto endScope() -> void;

    /// @brief Ends the compilation.
    auto endCompilation() -> cppLox::Types::ObjectFunction *;

    /// @brief Throws an exception at the previous token.
    /// @param message The message to display.
    auto error(char const * message) -> void;

    /// @brief Throws an exception at the previous token.
    /// @param message The message to display.
    auto error(std::string & message) -> void;

    /// @brief Throws an exception at the current token.
    /// @param message The message to display.
    auto errorAtCurrent(std::string & message) -> void;

    /// @brief Compiles an expression.
    /// @param tokens The tokens that are compiled.
    auto expression(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles an expression statement.
    /// @param tokens The tokens that are compiled.
    auto expressionStatement(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a for statement.
    /// @param tokens The tokens that are compiled.
    auto forStatement(std::vector<Token> const & tokens) -> void;

    auto funDeclaration(std::vector<Token> const & tokens) -> void;

    auto function(FunctionType type, std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a grouping expression.
    /// @param tokens The tokens that are compiled.
    auto grouping(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Gets the rule for the given token type.
    /// @param type The type of the token.
    /// @return The rule for the given token type.
    [[nodiscard]] auto getRule(Token::Type type) -> ParseRule<Compiler> *;

    /// @brief Compiles an identifier expression.
    /// @param name The name of the identifier.
    /// @return The index of the identifier in the chunk.
    [[nodiscard]] auto identifierConstant(Token const & name) -> uint8_t;

    /// @brief Compiles an if statement.
    /// @param tokens The tokens that are compiled.
    /// @return The index of the variable in the chunk.
    auto ifStatement(std::vector<Token> const & tokens) -> void;

    /// @brief Initializes the compiler scope.
    auto initCompiler(FunctionType type) -> void;

    /// @brief Compiles an literal expression.
    /// @param tokens The tokens that are compiled.
    auto literal(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Makes a constant from the given value.
    /// @param value The value to make a constant from.
    auto makeConstant(cppLox::Types::Value value) -> void;

    auto markInitialized() -> void;

    /// @brief Matches the current token with the given type and advances to the next token if it matches.
    /// @param type The type of the token.
    /// @param tokens The tokens that are compiled.
    /// @return Whether the current token matches the given type.
    [[nodiscard]] auto match(Token::Type type, std::vector<Token> const & tokens) -> bool;

    /// @brief Compiles variable name
    /// @param name The name of the variable.
    /// @param tokens The tokens that are compiled.
    /// @param canAssign Whether the variable can be assigned.
    auto namedVariable(Token const & name, std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Compiles a numerical constant.
    /// @param tokens The tokens that are compiled.
    auto number(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Compiles an or expression.
    /// @param tokens The tokens that are compiled.
    /// @return The index of the variable in the chunk.
    auto or_(std::vector<Token> const & tokens) -> void;

    /// @brief Parses the precedence of the current token.
    /// @param precedence The precedence to parse.
    /// @param tokens The tokens that are compiled.
    auto parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void;

    /// @brief Parses a variable.
    /// @param errorMessage The error message to display if no identifier is detected.
    /// @param tokens The tokens that are compiled.
    /// @return The index of the variable in the chunk.
    [[nodiscard]] auto parseVariable(std::string errorMessage, std::vector<Token> const & tokens) -> uint8_t;

    /// @brief Changes the size of the jump offset at the given offset in the chunk.
    /// @param offset The offset of the jump instruction.
    auto patchJump(int32_t offset) -> void;

    /// @brief Compiles a print statement.
    /// @param tokens The tokens that are compiled.
    auto printStatement(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Resolves a local variable.
    /// @param name The name of the variable.
    /// @param tokens The tokens that are compiled.
    /// @return The index of the variable in the chunk.
    [[nodiscard]] auto resolveLocal(Token const & name, LocalScope const & scope) -> int;

    auto returnStatement(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a statement.
    /// @param tokens The tokens that are compiled.
    /// @return The statement.
    auto statement(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a string constant.
    /// @param tokens The tokens that are compiled.
    auto string(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Synchronizes the compiler after an error.
    /// @param tokens The tokens that are compiled.
    auto synchronize(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a unary expression.
    /// @param tokens The tokens that are compiled.
    auto unary(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Compiles a variable name
    /// @param tokens The tokens that are compiled.
    /// @param canAssign Whether the variable can be assigned.
    auto variable(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Compiles a variable declaration.
    /// @param tokens The tokens that are compiled.
    auto variableDeclaration(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a while statement.
    /// @param tokens The tokens that are compiled.
    auto whileStatement(std::vector<Token> const & tokens) -> void;

    /// @brief Creates the parsing rules for the different token types.
    /// @return The rules for the different token types.
    static std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> makeRules() {
        std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> rules{};
        rules[static_cast<size_t>(Token::Type::LEFT_PARENTHESES)] =
            ParseRule<Compiler>(&Compiler::grouping, &Compiler::call, Precedence::CALL);
        rules[static_cast<size_t>(Token::Type::RIGHT_PARENTHESES)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::LEFT_BRACE)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::RIGHT_BRACE)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::COMMA)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::DOT)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::MINUS)] =
            ParseRule<Compiler>(&Compiler::unary, &Compiler::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::PLUS)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::SEMICOLON)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::SLASH)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::FACTOR);
        rules[static_cast<size_t>(Token::Type::STAR)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::FACTOR);
        rules[static_cast<size_t>(Token::Type::BANG)] =
            ParseRule<Compiler>(&Compiler::unary, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::BANG_EQUAL)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::EQUAL)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::EQUAL_EQUAL)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::GREATER)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::GREATER_EQUAL)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS_EQUAL)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::IDENTIFIER)] =
            ParseRule<Compiler>(&Compiler::variable, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::STRING)] =
            ParseRule<Compiler>(&Compiler::string, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::NUMBER)] =
            ParseRule<Compiler>(&Compiler::number, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::AND)] =
            ParseRule<Compiler>(std::nullopt, &Compiler::and_, Precedence::AND);
        rules[static_cast<size_t>(Token::Type::CLASS)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::ELSE)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FALSE)] =
            ParseRule<Compiler>(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FUN)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FOR)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::IF)] = ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::NULL_)] =
            ParseRule<Compiler>(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::OR)] = ParseRule<Compiler>(std::nullopt, &Compiler::or_, Precedence::OR);
        rules[static_cast<size_t>(Token::Type::PRINT)] =
            ParseRule<Compiler>(&Compiler::printStatement, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::RETURN)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::SUPER)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::THIS)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::TRUE)] =
            ParseRule<Compiler>(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::VAR)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::WHILE)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::END_OF_FILE)] =
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
        return rules;
    }

    /// @brief The token that was previously compiled.
    Token const * m_previous;
    /// @brief The token that is currently compiled.
    Token const * m_current;
    /// @brief The index of the current token.
    size_t m_currentTokenIndex;
    /// @brief The memory manager.
    std::shared_ptr<cppLox::MemoryMutator> m_memoryMutator;
    /// @brief The rules for the different token types.
    static inline std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> m_rules = makeRules();
    /// @brief Whether the compiler is in panic mode.
    bool m_panicMode;
    /// Whether the compilation resulted in an error.
    bool m_hadError;
    std::shared_ptr<CompilationScope> m_currentScope;
};
} // namespace cppLox::Frontend