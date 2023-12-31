#include <gtest/gtest.h>

#include <format>

#include "../../src/frontend/token.hpp"
#include "../../src/frontend/token_formatter.hpp"

cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::NUMBER, "3.14", 1);

TEST(TokenTest, Type) {
    ASSERT_EQ(token.type(), cppLox::Frontend::Token::Type::NUMBER);
}

TEST(TokenTest, Lexeme) {
    ASSERT_EQ(token.lexeme(), "3.14");
}

TEST(TokenTest, Line) {
    ASSERT_EQ(token.line(), 1);
}

TEST(TokenTest, ExtractionOperator) {
    // Arrange
    testing::internal::CaptureStdout();
    // Act
    std::cout << token;
    // Assert
    ASSERT_EQ("Token(3.14, 1)", testing::internal::GetCapturedStdout());
}

TEST(TokenTest, Formatter) {
    ASSERT_EQ("Token(3.14, 1)", std::format("{}", token));
}