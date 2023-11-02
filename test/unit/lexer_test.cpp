#include "../../src/frontend/lexer.hpp"

#include <string>
#include <vector>

#include <gtest/gtest.h>

// Test fixture for Lexer tests
class LexerTest : public ::testing::Test {
  public:
    cppLox::Frontend::Lexer lexer;
    void SetUp() override {
        lexer = cppLox::Frontend::Lexer();
    }
};

TEST_F(LexerTest, ScanString) {
    // Arrange
    std::string source = "\"Hello, World!\"";

    // Act
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    cppLox::Frontend::Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), cppLox::Frontend::Token::Type::STRING);
    EXPECT_EQ(token.lexeme(), "Hello, World!");
}

TEST_F(LexerTest, CanHandleComment) {
    // Arrange
    std::string source = "// This is a comment";

    // Act
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    cppLox::Frontend::Token token = tokens[0];

    // Assert
    EXPECT_EQ(cppLox::Frontend::Token::Type::END_OF_FILE, token.type());
}

class SingleTokenLexerTest
    : public LexerTest,
      public ::testing::WithParamInterface<std::pair<std::string, cppLox::Frontend::Token::Type>> {
  public:
    std::string source;
    cppLox::Frontend::Token::Type expectedType;
    void SetUp() override {
        source = GetParam().first;
        expectedType = GetParam().second;
    }
};

INSTANTIATE_TEST_SUITE_P(ScanSingleToken, SingleTokenLexerTest,
                         ::testing::Values(std::make_pair("(", cppLox::Frontend::Token::Type::LEFT_PARENTHESES),
                                           std::make_pair(")", cppLox::Frontend::Token::Type::RIGHT_PARENTHESES),
                                           std::make_pair("{", cppLox::Frontend::Token::Type::LEFT_BRACE),
                                           std::make_pair("}", cppLox::Frontend::Token::Type::RIGHT_BRACE),
                                           std::make_pair(",", cppLox::Frontend::Token::Type::COMMA),
                                           std::make_pair(".", cppLox::Frontend::Token::Type::DOT),
                                           std::make_pair("-", cppLox::Frontend::Token::Type::MINUS),
                                           std::make_pair("+", cppLox::Frontend::Token::Type::PLUS),
                                           std::make_pair(";", cppLox::Frontend::Token::Type::SEMICOLON),
                                           std::make_pair("*", cppLox::Frontend::Token::Type::STAR),
                                           std::make_pair("!", cppLox::Frontend::Token::Type::BANG),
                                           std::make_pair("!=", cppLox::Frontend::Token::Type::BANG_EQUAL),
                                           std::make_pair("=", cppLox::Frontend::Token::Type::EQUAL),
                                           std::make_pair("==", cppLox::Frontend::Token::Type::EQUAL_EQUAL),
                                           std::make_pair(">", cppLox::Frontend::Token::Type::GREATER),
                                           std::make_pair(">=", cppLox::Frontend::Token::Type::GREATER_EQUAL),
                                           std::make_pair("<", cppLox::Frontend::Token::Type::LESS),
                                           std::make_pair("<=", cppLox::Frontend::Token::Type::LESS_EQUAL),
                                           std::make_pair("/", cppLox::Frontend::Token::Type::SLASH),
                                           std::make_pair("and", cppLox::Frontend::Token::Type::AND),
                                           std::make_pair("class", cppLox::Frontend::Token::Type::CLASS),
                                           std::make_pair("else", cppLox::Frontend::Token::Type::ELSE),
                                           std::make_pair("false", cppLox::Frontend::Token::Type::FALSE),
                                           std::make_pair("for", cppLox::Frontend::Token::Type::FOR),
                                           std::make_pair("fun", cppLox::Frontend::Token::Type::FUN),
                                           std::make_pair("if", cppLox::Frontend::Token::Type::IF),
                                           std::make_pair("null", cppLox::Frontend::Token::Type::NULL_),
                                           std::make_pair("or", cppLox::Frontend::Token::Type::OR),
                                           std::make_pair("print", cppLox::Frontend::Token::Type::PRINT),
                                           std::make_pair("return", cppLox::Frontend::Token::Type::RETURN),
                                           std::make_pair("super", cppLox::Frontend::Token::Type::SUPER),
                                           std::make_pair("this", cppLox::Frontend::Token::Type::THIS),
                                           std::make_pair("true", cppLox::Frontend::Token::Type::TRUE),
                                           std::make_pair("var", cppLox::Frontend::Token::Type::VAR),
                                           std::make_pair("while", cppLox::Frontend::Token::Type::WHILE),
                                           std::make_pair("123", cppLox::Frontend::Token::Type::NUMBER),
                                           std::make_pair("3.14", cppLox::Frontend::Token::Type::NUMBER),
                                           std::make_pair("foo", cppLox::Frontend::Token::Type::IDENTIFIER)));

TEST_P(SingleTokenLexerTest, ScanSingleToken) {
    // Act
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    cppLox::Frontend::Token token = tokens[0];

    // Assert
    EXPECT_EQ(expectedType, token.type());
    EXPECT_EQ(source, token.lexeme());
}
