#include <gtest/gtest.h>

#include <format>

#include "../../src/frontend/token.hpp"
#include "../../src/frontend/token_formatter.hpp"

cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::NUMBER, "3.14", 1);

TEST(TokenTest, Type) {
    // Act
    auto type = token.type();

    // Assert
    ASSERT_EQ(type, cppLox::Frontend::Token::Type::NUMBER);
}

TEST(TokenTest, Lexeme) {
    // Act
    auto lexeme = token.lexeme();

    // Assert
    ASSERT_EQ(lexeme, "3.14");
}

TEST(TokenTest, Line) {
    // Act
    auto line = token.line();

    // Assert
    ASSERT_EQ(line, token.line());
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
    // Act
    auto result = std::format("{}", token);

    // Assert
    ASSERT_EQ(result, "Token(3.14, 1)");
}