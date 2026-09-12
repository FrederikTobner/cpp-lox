#include "../../src/types/object_closure.hpp"
#include "../../src/types/object_formatter.hpp"

#include <gtest/gtest.h>

class ObjectClosureTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Types::Object> object;
    std::unique_ptr<cppLox::Types::ObjectString> functionName;
    std::unique_ptr<cppLox::Types::ObjectFunction> function;
    auto SetUp() -> void override {
        functionName = std::make_unique<cppLox::Types::ObjectString>("test");
        function = std::make_unique<cppLox::Types::ObjectFunction>(1, functionName.get());
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectClosure(function.get())));
    }
};

TEST_F(ObjectClosureTest, type) {
    // Act
    auto type = object->type();

    // Assert
    ASSERT_EQ(type, cppLox::Types::Object::Type::CLOSURE);
}

TEST_F(ObjectClosureTest, is) {
    // Act
    auto result = object->is(cppLox::Types::Object::Type::CLOSURE);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(ObjectClosureTest, function) {
    // Act
    auto result = object->as<cppLox::Types::ObjectClosure>()->function();

    // Assert
    ASSERT_EQ(result, function.get());
}

TEST_F(ObjectClosureTest, writeToOutputStream) {
    // Arrange
    std::stringstream ss;

    // Act
    object->writeToOutputStream(ss);

    // Assert
    ASSERT_EQ(ss.str(), "<fn test>");
}

TEST_F(ObjectClosureTest, format) {
    // Act
    auto result = std::format("{}", object.get());

    // Assert
    ASSERT_EQ(result, "<fn test>");
}