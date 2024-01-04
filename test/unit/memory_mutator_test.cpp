#include "../../src/memory_mutator.hpp"
#include "../../src/types/object.hpp"

#include <gtest/gtest.h>

class MemoryMutatorTest : public ::testing::Test {
  public:
    std::unique_ptr<cppLox::MemoryMutator> memoryMutator;

    MemoryMutatorTest() {
        memoryMutator = std::make_unique<cppLox::MemoryMutator>();
    }
};

TEST_F(MemoryMutatorTest, DoesNotCreateDuplicateStrings) {
    // Act
    cppLox::Types::Object * string1 = memoryMutator->create<cppLox::Types::ObjectString>("test");
    cppLox::Types::Object * string2 = memoryMutator->create<cppLox::Types::ObjectString>("test");

    // Assert
    EXPECT_EQ((void *)string1, (void *)string2);
}