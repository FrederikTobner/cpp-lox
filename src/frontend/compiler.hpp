#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../bytecode/chunk.hpp"
#include "../memory_mutator.hpp"
#include "precedence.hpp"
#include "token.hpp"

namespace cppLox::Frontend {

// Forward declaration of the compiler.
class Compiler;

class ParseRule {
  public:
    ParseRule() {
        m_prefix = std::nullopt;
        m_infix = std::nullopt;
        m_precedence = Precedence::NONE;
    }
    ParseRule(std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> prefix,
              std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> infix, Precedence precedence) {
        m_prefix = prefix;
        m_infix = infix;
        m_precedence = precedence;
    }
    ~ParseRule() = default;
    /// @brief Gets the prefix function of the rule.
    /// @return An optional containing the prefix function of the rule.
    [[nodiscard]] auto prefix() const -> std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> {
        return m_prefix;
    }
    /// @brief Gets the infix function of the rule.
    /// @return An optional containing the infix function of the rule.
    [[nodiscard]] auto infix() const -> std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> {
        return m_infix;
    }
    /// @brief Gets the precedence of the rule.
    /// @return The precedence of the rule.
    [[nodiscard]] auto precedence() -> Precedence const & {
        return m_precedence;
    }

  private:
    Precedence m_precedence;
    std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> m_prefix;
    std::optional<void (Compiler::*)(std::vector<Token> const & tokens)> m_infix;
};

class Compiler {

  public:
    /// @brief Constructs a new compiler.
    Compiler(MemoryMutator * memoryMutator);

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

    /// @brief Compiles a grouping expression.
    /// @param tokens The tokens that are compiled.
    auto grouping(std::vector<Token> const & tokens) -> void;

    /// @brief Gets the rule for the given token type.
    /// @param type The type of the token.
    /// @return The rule for the given token type.
    [[nodiscard]] auto getRule(Token::Type type) -> ParseRule *;

    /// @brief Compiles an literal expression.
    /// @param tokens The tokens that are compiled.
    auto literal(std::vector<Token> const & tokens) -> void;

    /// @brief Makes a constant from the given value.
    /// @param value The value to make a constant from.
    auto makeConstant(cppLox::Types::Value value) -> void;

    /// @brief Compiles a numerical constant.
    /// @param tokens The tokens that are compiled.
    auto number(std::vector<Token> const & tokens) -> void;

    /// @brief Parses the precedence of the current token.
    /// @param precedence The precedence to parse.
    /// @param tokens The tokens that are compiled.
    auto parsePrecedence(Precedence precedence, std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a print statement.
    /// @param tokens The tokens that are compiled.
    auto printStatement(std::vector<Token> const & tokens) -> void;

    /// @brief Compiles a unary expression.
    /// @param tokens The tokens that are compiled.
    auto unary(std::vector<Token> const & tokens) -> void;

    auto string(std::vector<Token> const & tokens) -> void;

    static std::array<ParseRule, static_cast<size_t>(Token::Type::AMOUNT)> makeRules() {
        std::array<ParseRule, static_cast<size_t>(Token::Type::AMOUNT)> rules{};
        rules[static_cast<size_t>(Token::Type::LEFT_PARENTHESES)] =
            ParseRule(&Compiler::grouping, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::RIGHT_PARENTHESES)] =
            ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::LEFT_BRACE)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::RIGHT_BRACE)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::COMMA)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::DOT)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::MINUS)] =
            ParseRule(&Compiler::unary, &Compiler::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::PLUS)] = ParseRule(std::nullopt, &Compiler::binary, Precedence::TERM);
        rules[static_cast<size_t>(Token::Type::SEMICOLON)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::SLASH)] = ParseRule(std::nullopt, &Compiler::binary, Precedence::FACTOR);
        rules[static_cast<size_t>(Token::Type::STAR)] = ParseRule(std::nullopt, &Compiler::binary, Precedence::FACTOR);
        rules[static_cast<size_t>(Token::Type::BANG)] = ParseRule(&Compiler::unary, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::BANG_EQUAL)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::EQUAL)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::EQUAL_EQUAL)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::EQUALITY);
        rules[static_cast<size_t>(Token::Type::GREATER)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::GREATER_EQUAL)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::LESS_EQUAL)] =
            ParseRule(std::nullopt, &Compiler::binary, Precedence::COMPARISON);
        rules[static_cast<size_t>(Token::Type::IDENTIFIER)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::STRING)] = ParseRule(&Compiler::string, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::NUMBER)] = ParseRule(&Compiler::number, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::AND)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::CLASS)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::ELSE)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FALSE)] = ParseRule(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FUN)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::FOR)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::IF)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::NULL_)] = ParseRule(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::OR)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::PRINT)] =
            ParseRule(&Compiler::printStatement, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::RETURN)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::SUPER)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::THIS)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::TRUE)] = ParseRule(&Compiler::literal, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::VAR)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::WHILE)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
        rules[static_cast<size_t>(Token::Type::END_OF_FILE)] = ParseRule(std::nullopt, std::nullopt, Precedence::NONE);
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
    MemoryMutator * m_memoryMutator;
    /// @brief The rules for the different token types.
    static inline std::array<ParseRule, static_cast<size_t>(Token::Type::AMOUNT)> m_rules = makeRules();
};
} // namespace cppLox::Frontend