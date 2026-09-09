#include "../../src/types/object_formatter.hpp"
#include "../../src/types/object_upvalue.hpp"

#include <gtest/gtest.h>

class ObjectUpvalueTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Types::Object> object;
    std::unique_ptr<cppLox::Types::Value> closed;
    auto SetUp() -> void override {
        closed = std::make_unique<cppLox::Types::Value>(1.0);
        object = std::unique_ptr<cppLox::Types::Object>(
            static_cast<cppLox::Types::Object *>(new cppLox::Types::ObjectUpValue(closed.get())));
    }
};

TEST_F(ObjectUpvalueTest, type) {
    // Act
    auto type = object->type();

    // Assert
    ASSERT_EQ(type, cppLox::Types::Object::Type::UPVALUE);
}

TEST_F(ObjectUpvalueTest, is) {
    // Act
    auto result = object->is(cppLox::Types::Object::Type::UPVALUE);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(ObjectUpvalueTest, location) {
    // Act
    auto result = object->as<cppLox::Types::ObjectUpValue>()->location();

    // Assert
    ASSERT_EQ(result, closed.get());
}

TEST_F(ObjectUpvalueTest, setLocation) {
    // Arrange
    auto newLocation = cppLox::Types::Value(1.0);

    // Act
    object->as<cppLox::Types::ObjectUpValue>()->setLocation(&newLocation);

    // Assert
    ASSERT_EQ(object->as<cppLox::Types::ObjectUpValue>()->location(), &newLocation);
}

TEST_F(ObjectUpvalueTest, isClosedInitiallyFalse) {
    // Act
    auto result = object->as<cppLox::Types::ObjectUpValue>()->isClosed();

    // Assert
    ASSERT_FALSE(result);
}

TEST_F(ObjectUpvalueTest, close) {
    // Arrange
    *closed = cppLox::Types::Value(42.0);
    auto * upvalue = object->as<cppLox::Types::ObjectUpValue>();

    // Act
    upvalue->close();

    // Assert
    ASSERT_TRUE(upvalue->isClosed());
    ASSERT_NE(upvalue->location(), closed.get());
    ASSERT_EQ(upvalue->location()->as<double>(), 42.0);
}

TEST_F(ObjectUpvalueTest, closeIsIndependentOfOriginalLocation) {
    // Arrange
    auto * upvalue = object->as<cppLox::Types::ObjectUpValue>();
    upvalue->close();

    // Act - mutating the original location must no longer affect the upvalue.
    *closed = cppLox::Types::Value(99.0);

    // Assert
    ASSERT_EQ(upvalue->location()->as<double>(), 1.0);
}

TEST_F(ObjectUpvalueTest, writeToOutputStream) {
    // Arrange
    std::stringstream ss;

    // Act
    object->writeToOutputStream(ss);

    // Assert
    ASSERT_EQ(ss.str(), "upvalue");
}

TEST_F(ObjectUpvalueTest, format) {
    // Act
    auto result = std::format("{}", object.get());

    // Assert
    ASSERT_EQ(result, "upvalue");
}