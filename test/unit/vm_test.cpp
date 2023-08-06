#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/value.hpp"
#include "../../src/vm.hpp"

#include <gtest/gtest.h>

// Test fixture for VM unit tests
class VMTest : public ::testing::Test {
  public:
    VM vm;
    void SetUp() override {
        vm = VM();
    }
};

TEST_F(VMTest, PushAndPop) {
    // Arrange
    Value value(42.0);

    // Act
    vm.push(value);
    Value result = vm.pop();

    // Assert
    EXPECT_EQ(value, result);
}