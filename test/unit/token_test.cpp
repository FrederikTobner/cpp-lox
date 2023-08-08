#include "../../src/token.hpp"

#include <gtest/gtest.h>

TEST(TokenTest, Type) {
    Token token(Token::Type::NUMBER, "3.14", 1);
    ASSERT_EQ(token.type(), Token::Type::NUMBER);
}

TEST(TokenTest, Lexeme) {
    Token token(Token::Type::NUMBER, "3.14", 1);
    ASSERT_EQ(token.lexeme(), "3.14");
}

TEST(TokenTest, Line) {
    Token token(Token::Type::NUMBER, "3.14", 1);
    ASSERT_EQ(token.line(), 1);
}
