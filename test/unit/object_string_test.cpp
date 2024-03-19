#include "../../src/types/object_formatter.hpp"
#include "../../src/types/object_string.hpp"

#include <memory>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

// Test fixture for ObjectString tests.
class ObjectStringTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Types::Object> object;
    auto SetUp() -> void override {
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectString("Hello World")));
    }
};

TEST_F(ObjectStringTest, type) {
    // Act
    auto type = object->type();

    // Assert
    ASSERT_EQ(type, cppLox::Types::Object::Type::STRING);
}

TEST_F(ObjectStringTest, is) {
    // Act
    auto result = object->is(cppLox::Types::Object::Type::STRING);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(ObjectStringTest, string) {
    // Assert
    ASSERT_EQ(object->type(), cppLox::Types::Object::Type::STRING);
    ASSERT_EQ(object->as<cppLox::Types::ObjectString>()->string(), "Hello World");
}

TEST_F(ObjectStringTest, ExtractionOperator) {
    // Arrange
    std::ostringstream oss;

    // Act
    oss << object;

    // Assert
    ASSERT_EQ("Hello World", oss.str());
}

TEST_F(ObjectStringTest, Formatter) {
    // Act
    auto result = std::format("{}", object.get());

    // Assert
    ASSERT_EQ(result, "Hello World");
}
