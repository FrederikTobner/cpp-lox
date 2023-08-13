#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "chunk.hpp"
#include "precedence.hpp"
#include "token.hpp"

class Compiler {
    typedef void (Compiler::*Parse_func)(std::vector<Token> & tokens);
    class ParseRule {
      public:
        ParseRule() {
            m_prefix = nullptr;
            m_infix = nullptr;
            m_precedence = Precedence::NONE;
        }
        ParseRule(Parse_func prefix, Parse_func infix, Precedence precedence) {
            m_prefix = prefix;
            m_infix = infix;
            m_precedence = precedence;
        }
        ~ParseRule() {
        }
        Parse_func prefix() {
            return m_prefix;
        }
        Parse_func infix() {
            return m_infix;
        }
        const Precedence & precedence() {
            return m_precedence;
        }

      private:
        Parse_func m_prefix;
        Parse_func m_infix;
        Precedence m_precedence;
    };

  public:
    /// @brief Constructs a new compiler.
    Compiler();

    /// @brief Destructor of the compiler.
    ~Compiler();

    /// @brief Compiles the given tokens.
    /// @param tokens The tokens that are compiled.
    /// @return The compiled chunk.
    std::unique_ptr<Chunk> compile(std::vector<Token> & tokens);

  private:
    /// @brief Advances to the next token.
    /// @param tokens The tokens that are compiled.
    void advance(std::vector<Token> & tokens);

    /// @brief Compiles a binary expression.
    /// @param tokens The tokens that are compiled.
    void binary(std::vector<Token> & tokens);

    /// @brief Consumes a token.
    /// @param type The type of the token.
    /// @param message The message to display if the token is not of the given type.
    /// @param tokens The tokens that are compiled.
    void consume(Token::Type type, std::string message, std::vector<Token> & tokens);

    /// @brief Gets the current chunk.
    /// @return The current chunk.
    Chunk * currentChunk();

    /// @brief Emits a byte.
    /// @param byte The byte to emit.
    void inline emitByte(uint8_t byte);

    /// @brief Emits two bytes.
    /// @param byte1 The first byte.
    /// @param byte2 The second byte.
    void emitBytes(uint8_t byte1, uint8_t byte2);

    /// @brief Emits a constant.
    void inline emitConstant(Value value);

    /// @brief Ends the compilation.
    void endCompilation();

    /// @brief Throws an exception at the previous token.
    /// @param message The message to display.
    void error(std::string message);

    /// @brief Throws an exception at the current token.
    /// @param message The message to display.
    void errorAtCurrent(std::string message);

    /// @brief Compiles an expression.
    /// @param tokens The tokens that are compiled.
    void expression(std::vector<Token> & tokens);

    /// @brief Compiles a grouping expression.
    /// @param tokens The tokens that are compiled.
    void grouping(std::vector<Token> & tokens);

    /// @brief Gets the rule for the given token type.
    /// @param type The type of the token.
    /// @return The rule for the given token type.
    ParseRule * getRule(Token::Type type);
    void literal(std::vector<Token> & tokens);

    /// @brief Makes a constant from the given value.
    /// @param value The value to make a constant from.
    void makeConstant(Value value);

    /// @brief Compiles a numerical constant.
    /// @param tokens The tokens that are compiled.
    void number(std::vector<Token> & tokens);

    /// @brief Parses the precedence of the current token.
    /// @param precedence The precedence to parse.
    /// @param tokens The tokens that are compiled.
    void parsePrecedence(Precedence precedence, std::vector<Token> & tokens);

    /// @brief Compiles a print statement.
    /// @param tokens The tokens that are compiled.
    void printStatement(std::vector<Token> & tokens);

    /// @brief Compiles a unary expression.
    /// @param tokens The tokens that are compiled.
    void unary(std::vector<Token> & tokens);

    /// @brief The token that was previously compiled.
    Token * m_previous;
    /// @brief The token that is currently compiled.
    Token * m_current;
    /// @brief The index of the current token.
    size_t m_currentTokenIndex;
    /// @brief The chunk that is currently compiled.
    Chunk * m_chunk;
    /// @brief The rules for the different token types.
    ParseRule m_rules[Token::WHILE + 1];
};