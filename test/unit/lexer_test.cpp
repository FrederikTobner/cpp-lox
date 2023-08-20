#include "../../src/frontend/lexer.hpp"

#include <string>
#include <vector>

#include <gtest/gtest.h>

// Test fixture for Lexer tests
class LexerTest : public ::testing::Test {
  public:
    Lexer lexer;
    void SetUp() override {
        lexer = Lexer();
    }
};

TEST_F(LexerTest, ScanNumber) {
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

TEST_F(LexerTest, ScanString) {
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

TEST_F(LexerTest, ScanIdentifier) {
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

TEST_F(LexerTest, ScanAndKeyword) {
    // Arrange
    std::string source = "and";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::AND);
    EXPECT_EQ(token.lexeme(), "and");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanClassKeyword) {
    // Arrange
    std::string source = "class";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::CLASS);
    EXPECT_EQ(token.lexeme(), "class");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanElseKeyword) {
    // Arrange
    std::string source = "else";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::ELSE);
    EXPECT_EQ(token.lexeme(), "else");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanForKeyword) {
    // Arrange
    std::string source = "for";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::FOR);
    EXPECT_EQ(token.lexeme(), "for");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanFunKeyword) {
    // Arrange
    std::string source = "fun";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::FUN);
    EXPECT_EQ(token.lexeme(), "fun");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanIfKeyword) {
    // Arrange
    std::string source = "if";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::IF);
    EXPECT_EQ(token.lexeme(), "if");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanNullKeyword) {
    // Arrange
    std::string source = "null";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::NULL_);
    EXPECT_EQ(token.lexeme(), "null");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanOrKeyword) {
    // Arrange
    std::string source = "or";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::OR);
    EXPECT_EQ(token.lexeme(), "or");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanPrintKeyword) {
    // Arrange
    std::string source = "print";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::PRINT);
    EXPECT_EQ(token.lexeme(), "print");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanReturnKeyword) {
    // Arrange
    std::string source = "return";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.type(), Token::Type::RETURN);
    EXPECT_EQ(token.lexeme(), "return");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanSuperKeyword) {
    // Arrange
    std::string source = "super";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(token.lexeme(), "super");
    EXPECT_EQ(token.type(), Token::Type::SUPER);
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTest, ScanThisKeyword) {
    // Arrange
    std::string source = "this";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("this", token.lexeme());
    EXPECT_EQ(Token::Type::THIS, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanTrueKeyword) {
    // Arrange
    std::string source = "true";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("true", token.lexeme());
    EXPECT_EQ(Token::Type::TRUE, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanVarKeyword) {
    // Arrange
    std::string source = "var";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("var", token.lexeme());
    EXPECT_EQ(Token::Type::VAR, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanWhileKeyword) {
    // Arrange
    std::string source = "while";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("while", token.lexeme());
    EXPECT_EQ(Token::Type::WHILE, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanLeftParen) {
    // Arrange
    std::string source = "(";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("(", token.lexeme());
    EXPECT_EQ(Token::Type::LEFT_PARENTHESES, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanRightParen) {
    // Arrange
    std::string source = ")";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(")", token.lexeme());
    EXPECT_EQ(Token::Type::RIGHT_PARENTHESES, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanLeftBrace) {
    // Arrange
    std::string source = "{";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("{", token.lexeme());
    EXPECT_EQ(Token::Type::LEFT_BRACE, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanRightBrace) {
    // Arrange
    std::string source = "}";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("}", token.lexeme());
    EXPECT_EQ(Token::Type::RIGHT_BRACE, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanComma) {
    // Arrange
    std::string source = ",";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(",", token.lexeme());
    EXPECT_EQ(Token::Type::COMMA, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanDot) {
    // Arrange
    std::string source = ".";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(".", token.lexeme());
    EXPECT_EQ(Token::Type::DOT, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanMinus) {
    // Arrange
    std::string source = "-";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("-", token.lexeme());
    EXPECT_EQ(Token::Type::MINUS, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanPlus) {
    // Arrange
    std::string source = "+";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("+", token.lexeme());
    EXPECT_EQ(Token::Type::PLUS, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanSlash) {
    // Arrange
    std::string source = "/";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("/", token.lexeme());
    EXPECT_EQ(Token::Type::SLASH, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanStar) {
    // Arrange
    std::string source = "*";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("*", token.lexeme());
    EXPECT_EQ(Token::Type::STAR, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanSemicolon) {
    // Arrange
    std::string source = ";";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(";", token.lexeme());
    EXPECT_EQ(Token::Type::SEMICOLON, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanBang) {
    // Arrange
    std::string source = "!";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("!", token.lexeme());
    EXPECT_EQ(Token::Type::BANG, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanBangEqual) {
    // Arrange
    std::string source = "!=";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("!=", token.lexeme());
    EXPECT_EQ(Token::Type::BANG_EQUAL, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanEqual) {
    // Arrange
    std::string source = "=";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("=", token.lexeme());
    EXPECT_EQ(Token::Type::EQUAL, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanEqualEqual) {
    // Arrange
    std::string source = "==";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("==", token.lexeme());
    EXPECT_EQ(Token::Type::EQUAL_EQUAL, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanGreater) {
    // Arrange
    std::string source = ">";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(">", token.lexeme());
    EXPECT_EQ(Token::Type::GREATER, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanGreaterEqual) {
    // Arrange
    std::string source = ">=";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(">=", token.lexeme());
    EXPECT_EQ(Token::Type::GREATER_EQUAL, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanLess) {
    // Arrange
    std::string source = "<";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("<", token.lexeme());
    EXPECT_EQ(Token::Type::LESS, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, ScanLessEqual) {
    // Arrange
    std::string source = "<=";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ("<=", token.lexeme());
    EXPECT_EQ(Token::Type::LESS_EQUAL, token.type());
    EXPECT_EQ(1, token.line());
}

TEST_F(LexerTest, CanHandleComment) {
    // Arrange
    std::string source = "// This is a comment";

    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];

    // Assert
    EXPECT_EQ(Token::Type::END_OF_FILE, token.type());
}