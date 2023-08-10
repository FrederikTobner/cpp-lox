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

TEST_F(LexerTestIT, ScanAndKeyword) {
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

TEST_F(LexerTestIT, ScanClassKeyword) {
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

TEST_F(LexerTestIT, ScanElseKeyword) {
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

TEST_F(LexerTestIT, ScanForKeyword) {
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

TEST_F(LexerTestIT, ScanFunKeyword) {
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

TEST_F(LexerTestIT, ScanIfKeyword) {
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

TEST_F(LexerTestIT, ScanNullKeyword) {
    // Arrange
    std::string source = "null";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::NULL_);
    EXPECT_EQ(token.lexeme(), "null");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanOrKeyword) {
    // Arrange
    std::string source = "or";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::OR);
    EXPECT_EQ(token.lexeme(), "or");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanPrintKeyword) {
    // Arrange
    std::string source = "print";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::PRINT);
    EXPECT_EQ(token.lexeme(), "print");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanReturnKeyword) {
    // Arrange
    std::string source = "return";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.type(), Token::RETURN);
    EXPECT_EQ(token.lexeme(), "return");
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanSuperKeyword) {
    // Arrange
    std::string source = "super";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(token.lexeme(), "super");
    EXPECT_EQ(token.type(), Token::SUPER);
    EXPECT_EQ(token.line(), 1);
}

TEST_F(LexerTestIT, ScanThisKeyword) {
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

TEST_F(LexerTestIT, ScanTrueKeyword) {
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

TEST_F(LexerTestIT, ScanVarKeyword) {
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

TEST_F(LexerTestIT, ScanWhileKeyword) {
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

TEST_F(LexerTestIT, ScanLeftParen) {
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

TEST_F(LexerTestIT, ScanRightParen) {
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

TEST_F(LexerTestIT, ScanLeftBrace) {
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

TEST_F(LexerTestIT, ScanRightBrace) {
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

TEST_F(LexerTestIT, ScanComma) {
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

TEST_F(LexerTestIT, ScanDot) {
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

TEST_F(LexerTestIT, ScanMinus) {
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

TEST_F(LexerTestIT, ScanPlus) {
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

TEST_F(LexerTestIT, ScanSlash) {
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

TEST_F(LexerTestIT, ScanStar) {
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

TEST_F(LexerTestIT, ScanSemicolon) {
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

TEST_F(LexerTestIT, ScanBang) {
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

TEST_F(LexerTestIT, ScanBangEqual) {
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

TEST_F(LexerTestIT, ScanEqual) {
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

TEST_F(LexerTestIT, ScanEqualEqual) {
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

TEST_F(LexerTestIT, ScanGreater) {
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

TEST_F(LexerTestIT, ScanGreaterEqual) {
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

TEST_F(LexerTestIT, ScanLess) {
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

TEST_F(LexerTestIT, ScanLessEqual) {
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

TEST_F(LexerTestIT, CanHandleComment) {
    // Arrange
    std::string source = "// This is a comment";
    // Act
    std::vector<Token> tokens = lexer.tokenize(source);
    Token token = tokens[0];
    // Assert
    EXPECT_EQ(Token::Type::END_OF_FILE, token.type());
}