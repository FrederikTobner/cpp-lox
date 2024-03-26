#include "../../src/types/object_formatter.hpp"
#include "../../src/types/object_native_fuction.hpp"

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

static void onError(cppLox::Backend::CallFrame & frame, std::string_view format) {
}

static auto testFunction(int argCount, cppLox::Types::Value * args, cppLox::Backend::CallFrame & frame,
                         std::function<void(cppLox::Backend::CallFrame & frame, std::string_view format)> onError)
    -> cppLox::Types::Value {
    return cppLox::Types::Value();
}

// Test fixture for ObjectNativeFunction tests.
class ObjectNativeFunctionTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Types::Object> object;
    auto SetUp() -> void override {
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectNativeFunction(testFunction, 0)));
    }
};

TEST_F(ObjectNativeFunctionTest, type) {
    // Act
    auto type = object->type();

    // Assert
    ASSERT_EQ(type, cppLox::Types::Object::Type::NATIVE_FUNCTION);
}

TEST_F(ObjectNativeFunctionTest, is) {
    // Act
    auto result = object->is(cppLox::Types::Object::Type::NATIVE_FUNCTION);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(ObjectNativeFunctionTest, arity) {
    // Act
    auto result = object->as<cppLox::Types::ObjectNativeFunction>()->arity();

    // Assert
    ASSERT_EQ(result, 0);
}

TEST_F(ObjectNativeFunctionTest, call) {
    // Arrange
    auto callFrame = cppLox::Backend::CallFrame();

    // Act
    auto result = object->as<cppLox::Types::ObjectNativeFunction>()->call(0, nullptr, callFrame, onError);

    // Assert
    ASSERT_EQ(result, cppLox::Types::Value());
}

TEST_F(ObjectNativeFunctionTest, writeToOutputStream) {
    // Arrange
    std::ostringstream oss;

    // Act
    object->writeToOutputStream(oss);

    // Assert
    ASSERT_EQ("<native fn>", oss.str());
}

TEST_F(ObjectNativeFunctionTest, format) {
    // Act
    auto result = std::format("{}", object.get());

    // Assert
    ASSERT_EQ(result, "<native fn>");
}