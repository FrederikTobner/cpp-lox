#include <gtest/gtest.h>

#include "../../src/frontend/local_scope.hpp"
#include "../../src/frontend/token.hpp"

class LocalScopeTest : public ::testing::Test {
  protected:
    void SetUp() override {
        localScope = std::make_shared<cppLox::Frontend::LocalScope>();
    }

    std::shared_ptr<cppLox::Frontend::LocalScope> localScope;
};

TEST_F(LocalScopeTest, AddLocal) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    // Act
    localScope->addLocal(token, [](std::string &) {});

    // Assert
    ASSERT_EQ(localScope->localCount(), 1);
    auto local = localScope->local(0);
    EXPECT_EQ(local.getToken().lexeme(), "test");
    EXPECT_EQ(local.getToken().line(), 123);
    EXPECT_EQ(local.getDepth(), -1);
}

TEST_F(LocalScopeTest, Enclosing) {
    // Arrange
    auto nestedCompilationScope = cppLox::Frontend::LocalScope(localScope);

    // Act
    auto result = nestedCompilationScope.enclosing().value().get();

    // Assert
    ASSERT_EQ(result, localScope.get());
}

TEST_F(LocalScopeTest, MarkInitialized) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    localScope->addLocal(token, [](std::string &) {});

    // Act
    localScope->markInitialized(0);

    // Assert
    ASSERT_EQ(localScope->localCount(), 1);
    auto local = localScope->local(0);
    EXPECT_EQ(local.getDepth(), 0);
}

TEST_F(LocalScopeTest, PopLocal) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    localScope->addLocal(token, [](std::string &) {});

    // Act
    localScope->popLocal();

    // Assert
    ASSERT_EQ(localScope->localCount(), 0);
}
