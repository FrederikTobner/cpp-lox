#include "../../src/types/object_formatter.hpp"
#include "../../src/types/object_function.hpp"

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

// Test fixture for ObjectNativeFunction tests.
class ObjectFunctionTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Types::Object> object;
    std::unique_ptr<cppLox::Types::ObjectString> functionName = std::make_unique<cppLox::Types::ObjectString>("test");
    auto SetUp() -> void override {
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectFunction(1, functionName.get())));
    }
};

TEST_F(ObjectFunctionTest, type) {
    // Act
    auto type = object->type();

    // Assert
    ASSERT_EQ(type, cppLox::Types::Object::Type::FUNCTION);
}

TEST_F(ObjectFunctionTest, is) {
    // Act
    auto result = object->is(cppLox::Types::Object::Type::FUNCTION);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(ObjectFunctionTest, arity) {
    // Act
    auto result = object->as<cppLox::Types::ObjectFunction>()->arity();

    // Assert
    ASSERT_EQ(result, 1);
}

TEST_F(ObjectFunctionTest, chunk) {
    // Act
    auto result = object->as<cppLox::Types::ObjectFunction>()->chunk();

    // Assert
    ASSERT_NE(result, nullptr);
}

TEST_F(ObjectFunctionTest, name) {
    // Act
    auto result = object->as<cppLox::Types::ObjectFunction>()->name();

    // Assert
    ASSERT_EQ(result, functionName.get());
}

TEST_F(ObjectFunctionTest, incrementArity) {
    // Act
    object->as<cppLox::Types::ObjectFunction>()->incrementArity();

    // Assert
    ASSERT_EQ(object->as<cppLox::Types::ObjectFunction>()->arity(), 2);
}

TEST_F(ObjectFunctionTest, writeToOutputStream) {
    // Arrange
    std::ostringstream oss;

    // Act
    object->writeToOutputStream(oss);

    // Assert
    ASSERT_EQ(oss.str(), "<fn test>");
}

TEST_F(ObjectFunctionTest, format) {
    // Act
    auto result = std::format("{}", object.get());

    // Assert
    ASSERT_EQ(result, "<fn test>");
}
