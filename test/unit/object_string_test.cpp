#include "../../src/types/object_formatter.hpp"
#include "../../src/types/object_string.hpp"

#include <memory>
#include <string>

#include <gtest/gtest.h>

// Test fixture for ObjectString tests.
class ObjectStringTest : public ::testing::Test {
  public:
    std::unique_ptr<cppLox::Types::Object> object;

  protected:
    // Sets up the test fixture.
    void SetUp() override {
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectString("Hello World")));
    }
};

TEST_F(ObjectStringTest, type) {
    ASSERT_EQ(object->type(), cppLox::Types::Object::Type::STRING);
}

TEST_F(ObjectStringTest, is) {
    ASSERT_TRUE(object->is(cppLox::Types::Object::Type::STRING));
}

TEST_F(ObjectStringTest, string) {
    ASSERT_EQ(object->type(), cppLox::Types::Object::Type::STRING);
    ASSERT_EQ(object->as<cppLox::Types::ObjectString>()->string(), "Hello World");
}

TEST_F(ObjectStringTest, ExtractionOperator) {
    // Arrange
    testing::internal::CaptureStdout();
    // Act
    std::cout << object;
    // Assert
    ASSERT_EQ("Hello World", testing::internal::GetCapturedStdout());
}

TEST_F(ObjectStringTest, Formatter) {
    ASSERT_EQ(std::format("{}", object.get()), "Hello World");
}