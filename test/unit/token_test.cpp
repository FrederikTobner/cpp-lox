#include "../../src/token.hpp"

#include <gtest/gtest.h>

Token token(Token::Type::NUMBER, "3.14", 1);

TEST(TokenTest, Type) {
    ASSERT_EQ(token.type(), Token::Type::NUMBER);
}

TEST(TokenTest, Lexeme) {
    ASSERT_EQ(token.lexeme(), "3.14");
}

TEST(TokenTest, Line) {
    ASSERT_EQ(token.line(), 1);
}