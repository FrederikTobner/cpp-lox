#include <gtest/gtest.h>

#include "../../src/frontend/compilation_scope.hpp"
#include "../../src/frontend/token.hpp"

class CompilationScopeTest : public ::testing::Test {
  protected:
    std::shared_ptr<cppLox::Frontend::CompilationScope> compilationScope;
    void SetUp() override {
        compilationScope = std::make_shared<cppLox::Frontend::CompilationScope>();
    }
};

TEST_F(CompilationScopeTest, AddLocal) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    // Act
    compilationScope->addLocal(token, [](std::string &) {});

    // Assert
    ASSERT_EQ(compilationScope->localCount(), 1);
    auto local = compilationScope->local(0);
    EXPECT_EQ(local.getToken().lexeme(), "test");
    EXPECT_EQ(local.getToken().line(), 123);
    EXPECT_EQ(local.getDepth(), -1);
}

TEST_F(CompilationScopeTest, MarkInitialized) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    compilationScope->addLocal(token, [](std::string &) {});

    // Act
    compilationScope->markInitialized(0);

    // Assert
    ASSERT_EQ(compilationScope->localCount(), 1);
    auto local = compilationScope->local(0);
    EXPECT_EQ(local.getDepth(), 0);
}

TEST_F(CompilationScopeTest, PopLocal) {
    // Arrange
    cppLox::Frontend::Token token(cppLox::Frontend::Token::Type::IDENTIFIER, "test", 123);
    compilationScope->addLocal(token, [](std::string &) {});

    // Act
    compilationScope->popLocal();

    // Assert
    ASSERT_EQ(compilationScope->localCount(), 0);
}

TEST_F(CompilationScopeTest, Enclosing) {
    // Arrange
    auto nestedCompilationScope = cppLox::Frontend::CompilationScope(compilationScope);

    // Act & Assert
    ASSERT_EQ(nestedCompilationScope.enclosing().value().get(), compilationScope.get());
}