#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/runtime_exception.hpp"
#include "../../src/value.hpp"
#include "../../src/vm.hpp"

#include <iostream>

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
    ASSERT_EQ(value, result);
}

TEST_F(VMTest, StackOverflow) {
    // Arrange
    Value value(42.0);
    for (int i = 0; i < 256; i++) {
        vm.push(value);
    }
    // Act & Assert
    ASSERT_THROW(vm.push(value), RunTimeException);
}

TEST_F(VMTest, StackUnderflow) {
    // Act & Assert
    ASSERT_THROW({ auto val = vm.pop(); }, RunTimeException);
}