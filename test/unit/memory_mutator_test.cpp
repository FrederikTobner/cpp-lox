#include "../../src/memory_mutator.hpp"
#include "../../src/types/object.hpp"

#include <gtest/gtest.h>

class MemoryMutatorTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::MemoryMutator> memoryMutator;
    auto SetUp() -> void override {
        memoryMutator = std::make_unique<cppLox::MemoryMutator>();
    }
};

TEST_F(MemoryMutatorTest, DoesNotCreateDuplicateStrings) {
    // Act
    cppLox::Types::Object * string1 = memoryMutator->create<cppLox::Types::ObjectString>("test");
    cppLox::Types::Object * string2 = memoryMutator->create<cppLox::Types::ObjectString>("test");

    // Assert
    EXPECT_EQ(static_cast<void *>(string1), static_cast<void *>(string2));
}

TEST_F(MemoryMutatorTest, CanGetGlobal) {
    // Arrange
    auto globalName = std::make_unique<cppLox::Types::ObjectString>("foo");
    auto globalValue = cppLox::Types::Value(1.0);
    memoryMutator->setGlobal(globalName.get(), globalValue);

    // Act
    auto gottenValue = memoryMutator->getGlobal(globalName.get());

    // Assert
    EXPECT_EQ(globalValue, gottenValue);
}
