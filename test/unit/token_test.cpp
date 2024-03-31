#include <gtest/gtest.h>

#include "../../src/frontend/token.hpp"
#include "../../src/frontend/token_formatter.hpp"

class TokenTest : public ::testing::Test {
  protected:
    TokenTest() : token(cppLox::Frontend::Token::Type::NUMBER, "3.14", 1) {
    }

    cppLox::Frontend::Token token;
};

TEST_F(TokenTest, Type) {
    // Act
    auto type = token.type();

    // Assert
    ASSERT_EQ(type, cppLox::Frontend::Token::Type::NUMBER);
}

TEST_F(TokenTest, Lexeme) {
    // Act
    auto lexeme = token.lexeme();

    // Assert
    ASSERT_EQ(lexeme, "3.14");
}

TEST_F(TokenTest, Line) {
    // Act
    auto line = token.line();

    // Assert
    ASSERT_EQ(line, token.line());
}

TEST_F(TokenTest, ExtractionOperator) {
    // Arrange
    std::ostringstream oss;

    // Act
    oss << token;

    // Assert
    ASSERT_EQ("Token(3.14, 1)", oss.str());
}

TEST_F(TokenTest, Formatter) {
    // Act
    auto result = std::format("{}", token);

    // Assert
    ASSERT_EQ(result, "Token(3.14, 1)");
}
