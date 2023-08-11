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
    Compiler();
    ~Compiler();
    std::unique_ptr<Chunk> compile(std::vector<Token> & tokens);

  private:
    void error(std::string message);
    void errorAtCurrent(std::string message);
    void consume(Token::Type type, std::string message, std::vector<Token> & tokens);
    void advance(std::vector<Token> & tokens);
    void emitByte(uint8_t byte);
    Chunk * currentChunk();
    void emitBytes(uint8_t byte1, uint8_t byte2);
    void endCompilation();
    void expression(std::vector<Token> & tokens);
    void emitConstant(Value value);
    void number(std::vector<Token> & tokens);
    void makeConstant(Value value);
    void grouping(std::vector<Token> & tokens);
    void unary(std::vector<Token> & tokens);
    void parsePrecedence(Precedence precedence, std::vector<Token> & tokens);
    void binary(std::vector<Token> & tokens);
    ParseRule * getRule(Token::Type type);
    Token * m_previous;
    Token * m_current;
    size_t m_currentTokenIndex;
    Chunk m_chunk;
    ParseRule m_rules[Token::WHILE + 1];
};