#include "../../src/lexer.hpp"

#include <string>
#include <vector>

#include <gtest/gtest.h>

// Test fixture for Lexer unit tests
class LexerTestIT : public ::testing::Test {
  public:
    Lexer lexer;
    void SetUp() override {
        lexer = Lexer();
    }
};

TEST_F(LexerTestIT, ScanNumber) {
    // Arrange
    std::string source = "3.14";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::Type::NUMBER);
    EXPECT_EQ(token.lexeme(), "3.14");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanString) {
    // Arrange
    std::string source = "\"Hello, World!\"";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::Type::STRING);
    EXPECT_EQ(token.lexeme(), "Hello, World!");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanIdentifier) {
    // Arrange
    std::string source = "foo";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::Type::IDENTIFIER);
    EXPECT_EQ(token.lexeme(), "foo");
    EXPECT_EQ(token.line(), 1);
}