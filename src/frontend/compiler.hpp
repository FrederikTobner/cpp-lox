#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../bytecode/chunk.hpp"
#include "../memory_mutator.hpp"
#include "parse_rule.hpp"
#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

class Compiler {

  public:
    /// @brief Constructs a new compiler.
    Compiler(cppLox::MemoryMutator * memoryMutator);

    /// @brief Destructor of the compiler.
    ~Compiler() = default;

    /// @brief Compiles the given tokens.
    /// @param tokens The tokens that are compiled.
    /// @return The compiled chunk.
    [[nodiscard]] auto compile(std::vector<Token> const & tokens) -> std::unique_ptr<cppLox::ByteCode::Chunk>;

  private:
    /// @brief Advances to the next token.
    /// @param tokens The tokens that are compiled.
    auto advance(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a binary expression.
    /// @param tokens The tokens that are compiled.
    auto binary(std::vector<Token> const & tokens) -> void;

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

    /// @brief Emits a DEFINE_GLOBAL opcode and the index of the variable in the chunk.
    /// @param global The index of the variable in the chunk.
    auto defineVariable(uint8_t global) -> void;

    /// @brief Emits a byte.
    /// @param byte The byte to emit.
    auto inline emitByte(uint8_t byte) -> void;

    /// @brief Emits two bytes.
    /// @param byte1 The first byte.
    /// @param byte2 The second byte.
    auto emitBytes(uint8_t byte1, uint8_t byte2) -> void;

    /// @brief Emits an opcode and a byte.
    /// @param opcode1 The opcode to emit.
    /// @param opcode2 The byte to emit.
    auto emitBytes(cppLox::ByteCode::Opcode opcode, uint8_t byte) -> void;

    /// @brief Emits a byte.
    /// @param byte The byte to emit.
    auto inline emitByte(cppLox::ByteCode::Opcode byte) -> void;

    /// @brief Emits two bytes.
    /// @param byte1 The first byte.
    /// @param byte2 The second byte.
    auto emitBytes(cppLox::ByteCode::Opcode byte1, cppLox::ByteCode::Opcode byte2) -> void;

    /// @brief Emits a constant.
    auto inline emitConstant(cppLox::Types::Value value) -> void;

    /// @brief Ends the compilation.
    auto endCompilation() -> void;

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
    /// @return The expression statement.
    auto expressionStatement(std::vector<Token> const & tokens) -> void;

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

    /// @brief Compiles an literal expression.
    /// @param tokens The tokens that are compiled.
    auto literal(std::vector<Token> const & tokens, bool canAssign) -> void;

    /// @brief Makes a constant from the given value.
    /// @param value The value to make a constant from.
    auto makeConstant(cppLox::Types::Value value) -> void;

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

    /// @brief Parses the precedence of the current token.
    /// @param precedence The precedence to parse.
    /// @param tokens The tokens that are compiled.
    auto parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void;

    /// @brief Parses a variable.
    /// @param errorMessage The error message to display if no identifier is detected.
    /// @param tokens The tokens that are compiled.
    /// @return The index of the variable in the chunk.
    [[nodiscard]] auto parseVariable(std::string errorMessage, std::vector<Token> const & tokens) -> uint8_t;

    /// @brief Compiles a print statement.
    /// @param tokens The tokens that are compiled.
    auto printStatement(std::vector<Token> const & tokens, bool canAssign) -> void;

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

    /// @brief Creates the parsing rules for the different token types.
    /// @return The rules for the different token types.
    static std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> makeRules() {
        std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> rules{};
        rules[static_cast<size_t>(Token::Type::LEFT_PARENTHESES)] =
            ParseRule<Compiler>(&Compiler::grouping, std::nullopt, Precedence::NONE);
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
            ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
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
        rules[static_cast<size_t>(Token::Type::OR)] = ParseRule<Compiler>(std::nullopt, std::nullopt, Precedence::NONE);
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
    /// @brief The chunk that is currently compiled.
    cppLox::ByteCode::Chunk * m_chunk;
    /// @brief The memory manager.
    cppLox::MemoryMutator * m_memoryMutator;
    /// @brief The rules for the different token types.
    static inline std::array<ParseRule<Compiler>, static_cast<size_t>(Token::Type::AMOUNT)> m_rules = makeRules();
    /// @brief Whether the compiler is in panic mode.
    bool m_panicMode;
};
} // namespace cppLox::Frontend