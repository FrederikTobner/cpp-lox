#include "../../src/backend/callframe.hpp"

#include <gtest/gtest.h>

class CallframeTest : public ::testing::Test {
  protected:
    auto SetUp() -> void override {
        name = std::make_unique<cppLox::Types::ObjectString>("test");
        function = std::make_unique<cppLox::Types::ObjectFunction>(0, name.get());
        slots = std::make_unique<cppLox::Types::Value>(1.2);
        closure = std::make_unique<cppLox::Types::ObjectClosure>(function.get());
        callframe = std::make_unique<cppLox::Backend::CallFrame>(closure.get(), slots.get());
    }

    std::unique_ptr<cppLox::Types::ObjectString> name;
    std::unique_ptr<cppLox::Types::ObjectFunction> function;
    std::unique_ptr<cppLox::Types::ObjectClosure> closure;
    std::unique_ptr<cppLox::Types::Value> slots;
    std::unique_ptr<cppLox::Backend::CallFrame> callframe;
};

TEST_F(CallframeTest, Closure) {
    // Act
    auto result = callframe->closure();

    // Assert
    ASSERT_EQ(result, closure.get());
}

TEST_F(CallframeTest, InstructionPointer) {
    // Act
    auto result = callframe->instructionPointer();

    // Assert
    ASSERT_EQ(result, closure->function()->chunk()->code().data());
}

TEST_F(CallframeTest, Slots) {
    // Act
    auto result = callframe->slots();

    // Assert
    ASSERT_EQ(result, slots.get());
}