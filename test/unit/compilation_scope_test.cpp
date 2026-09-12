#include "../../src/frontend/compilation_scope.hpp"

#include <gtest/gtest.h>

class CompilationScopeTest : public ::testing::Test {
  protected:
    auto SetUp() -> void override {
        name = std::make_unique<cppLox::Types::ObjectString>("test");
        function = std::make_unique<cppLox::Types::ObjectFunction>(0, name.get());
        testScope = std::make_shared<cppLox::Frontend::CompilationScope>(function.get(),
                                                                         cppLox::Frontend::FunctionType::FUNCTION);
    }

    std::shared_ptr<cppLox::Frontend::CompilationScope> testScope;
    std::unique_ptr<cppLox::Types::ObjectFunction> function;
    std::unique_ptr<cppLox::Types::ObjectString> name;
};

TEST_F(CompilationScopeTest, Enclosing) {
    // Act
    auto enclosingScope = testScope->enclosing();

    // Assert
    ASSERT_EQ(enclosingScope, nullptr);
}

TEST_F(CompilationScopeTest, Function) {
    // Act
    auto function_pointer = testScope->function();

    // Assert
    ASSERT_EQ(function_pointer, function.get());
}

TEST_F(CompilationScopeTest, CurrentFunctionType) {
    // Act
    auto functionType = testScope->currentFunctionType();

    // Assert
    ASSERT_EQ(functionType, cppLox::Frontend::FunctionType::FUNCTION);
}

TEST_F(CompilationScopeTest, ScopeDepth) {
    // Act
    auto scopeDepth = testScope->scopeDepth();

    // Assert
    ASSERT_EQ(scopeDepth, 0);
}

TEST_F(CompilationScopeTest, LocalScope) {
    // Act
    auto result = testScope->localScope();

    // Assert
    ASSERT_NE(result, nullptr);
}

TEST_F(CompilationScopeTest, BeginNewScope) {
    // Act
    testScope->beginNewScope();

    // Assert
    ASSERT_EQ(testScope->scopeDepth(), 1);
    ASSERT_NE(testScope->localScope(), nullptr);
}

TEST_F(CompilationScopeTest, EndScope) {
    // Arrange
    testScope->beginNewScope();

    // Act
    testScope->endScope();

    // Assert
    ASSERT_EQ(testScope->scopeDepth(), 0);
    ASSERT_NE(testScope->localScope(), nullptr);
}

TEST_F(CompilationScopeTest, AddUpvalue) {
    auto count = testScope->addUpvalue(0, true);

    // Assert
    ASSERT_EQ(count, 0);
}

TEST_F(CompilationScopeTest, Upvalue) {
    // Arrange
    auto count = testScope->addUpvalue(0, true);

    // Act
    auto upvalue = testScope->upvalue(0);

    // Assert
    ASSERT_TRUE(upvalue.isLocal());
    ASSERT_EQ(upvalue.index(), 0);
}

TEST_F(CompilationScopeTest, AddUpvalueNonLocal) {
    // Act
    auto count = testScope->addUpvalue(0, false);

    // Assert
    ASSERT_EQ(count, 0);
    auto upvalue = testScope->upvalue(0);
    ASSERT_FALSE(upvalue.isLocal());
    ASSERT_EQ(upvalue.index(), 0);
}

TEST_F(CompilationScopeTest, AddUpvalueDeduplicatesExistingEntry) {
    // Arrange - the same local slot is captured by two different closures compiled in the same scope.
    auto firstIndex = testScope->addUpvalue(0, true);

    // Act
    auto secondIndex = testScope->addUpvalue(0, true);

    // Assert - no new upvalue slot should have been created for the duplicate.
    ASSERT_EQ(firstIndex, secondIndex);
}

TEST_F(CompilationScopeTest, AddUpvalueDistinctSlotsCreateDistinctEntries) {
    // Arrange
    auto firstIndex = testScope->addUpvalue(0, true);

    // Act
    auto secondIndex = testScope->addUpvalue(1, true);

    // Assert
    ASSERT_NE(firstIndex, secondIndex);
    ASSERT_EQ(testScope->upvalue(secondIndex).index(), 1);
}

TEST_F(CompilationScopeTest, CreateEnclosingScope) {

    // Act
    auto newInnerScope = std::make_unique<cppLox::Frontend::CompilationScope>(testScope, function.get(),
                                                                              cppLox::Frontend::FunctionType::FUNCTION);

    // Assert
    ASSERT_EQ(newInnerScope->enclosing(), testScope);
    ASSERT_EQ(newInnerScope->function(), function.get());
    ASSERT_EQ(newInnerScope->currentFunctionType(), cppLox::Frontend::FunctionType::FUNCTION);
    ASSERT_EQ(newInnerScope->scopeDepth(), 0);
    ASSERT_NE(newInnerScope->localScope(), nullptr);
}
