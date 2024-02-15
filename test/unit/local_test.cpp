#include <gtest/gtest.h>

#include "../../src/frontend/local.hpp"

class LocalTest : public ::testing::Test {
  protected:
    LocalTest() : token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123), local(token, 321) {
    }

    cppLox::Frontend::Token token;
    cppLox::Frontend::Local local;
};

TEST_F(LocalTest, GetToken) {
    // Act
    auto result = local.getToken();

    // Assert
    ASSERT_EQ(result.lexeme(), "test");
    ASSERT_EQ(result.line(), 123);
}

TEST_F(LocalTest, GetDepth) {
    // Act
    auto result = local.getDepth();

    // Assert
    ASSERT_EQ(result, 321);
}