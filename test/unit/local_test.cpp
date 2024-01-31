#include <gtest/gtest.h>

#include "../../src/frontend/local.hpp"

static cppLox::Frontend::Token token = cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);

static cppLox::Frontend::Local local = cppLox::Frontend::Local(token, 321);

TEST(LocalTest, GetToken) {
    // Act
    auto result = local.getToken();

    // Assert
    ASSERT_EQ(result.lexeme(), "test");
    ASSERT_EQ(result.line(), 123);
}

TEST(LocalTest, GetDepth) {
    // Act
    auto result = local.getDepth();

    // Assert
    ASSERT_EQ(result, 321);
}