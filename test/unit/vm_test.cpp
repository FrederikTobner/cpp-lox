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
    std::unique_ptr<VM> vm;
    std::unique_ptr<MemoryManager> memory_manager;
    Chunk chunk;

    VMTest() {
        memory_manager = std::make_unique<MemoryManager>();
        vm = std::make_unique<VM>(memory_manager.get());
    }

    void SetUp() override {
        vm->resetStack();
        chunk = Chunk();
    }
};

TEST_F(VMTest, AddInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::ADD, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value + value, vm->pop());
}

TEST_F(VMTest, ConstantInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value, vm->pop());
}

TEST_F(VMTest, DivideInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::DIVIDE, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value / value, vm->pop());
}

TEST_F(VMTest, EqualInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    Value value3(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 == 43
    chunk.write(Opcode::EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value3), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 == 42
    chunk.write(Opcode::EQUAL, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 == 42
    EXPECT_EQ(true, vm->pop());
    // 42 == 43
    EXPECT_EQ(false, vm->pop());
}

TEST_F(VMTest, FalseInstruction) {
    // Arrange
    chunk.write(Opcode::FALSE, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(Value(false), vm->pop());
}

TEST_F(VMTest, GreaterInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 > 43
    chunk.write(Opcode::GREATER, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 43 > 42
    chunk.write(Opcode::GREATER, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 > 42
    chunk.write(Opcode::GREATER, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 > 42
    EXPECT_EQ(false, vm->pop());
    // 43 > 42
    EXPECT_EQ(false, vm->pop());
    // 42 > 43
    EXPECT_EQ(true, vm->pop());
}

TEST_F(VMTest, GreaterEqualInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 43 >= 42
    chunk.write(Opcode::GREATER_EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 >= 43
    chunk.write(Opcode::GREATER_EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 >= 42
    chunk.write(Opcode::GREATER_EQUAL, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 >= 42
    EXPECT_EQ(true, vm->pop());
    // 42 >= 43
    EXPECT_EQ(false, vm->pop());
    // 43 >= 42
    EXPECT_EQ(true, vm->pop());
}

TEST_F(VMTest, LessInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 < 43
    chunk.write(Opcode::LESS, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 43 < 42
    chunk.write(Opcode::LESS, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 < 42
    chunk.write(Opcode::LESS, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 < 42
    EXPECT_EQ(false, vm->pop());
    // 42 < 43
    EXPECT_EQ(true, vm->pop());
    // 43 < 42
    EXPECT_EQ(false, vm->pop());
}

TEST_F(VMTest, LessEqualInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 43 <= 42
    chunk.write(Opcode::LESS_EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 <= 43
    chunk.write(Opcode::LESS_EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 <= 42
    chunk.write(Opcode::LESS_EQUAL, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 <= 42
    EXPECT_EQ(true, vm->pop());
    // 42 <= 43
    EXPECT_EQ(true, vm->pop());
    // 43 <= 42
    EXPECT_EQ(false, vm->pop());
}

TEST_F(VMTest, MultiplyInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::MULTIPLY, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value * value, vm->pop());
}

TEST_F(VMTest, NegateInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::NEGATE, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(-value, vm->pop());
}

TEST_F(VMTest, NotEqualInstruction) {
    // Arrange
    Value value(42.0);
    Value value2(43.0);
    Value value3(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 != 43
    chunk.write(Opcode::NOT_EQUAL, 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value3), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 != 42
    chunk.write(Opcode::NOT_EQUAL, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 != 42
    EXPECT_EQ(false, vm->pop());
    // 42 != 43
    EXPECT_EQ(true, vm->pop());
}

TEST_F(VMTest, NullInstruction) {
    // Arrange
    chunk.write(Opcode::NULL_, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(Value(), vm->pop());
}

TEST_F(VMTest, PushAndPop) {
    // Arrange
    Value value(42.0);

    // Act
    vm->push(value);
    Value result = vm->pop();

    // Assert
    ASSERT_EQ(value, result);
}

TEST_F(VMTest, ReturnInstruction) {
    // Arrange
    chunk.write(Opcode::RETURN, 0);

    // Act & Assert
    ASSERT_NO_THROW(vm->interpret(chunk));
}

TEST_F(VMTest, StackOverflow) {
    // Arrange
    Value value(42.0);
    for (int i = 0; i < 256; i++) {
        vm->push(value);
    }
    Chunk chunk;
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), RunTimeException);
}

TEST_F(VMTest, StackUnderflow) {
    Chunk chunk;
    chunk.write(Opcode::ADD, 0);
    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), RunTimeException);
}

TEST_F(VMTest, SubtractInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(Opcode::SUBTRACT, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value - value, vm->pop());
}

TEST_F(VMTest, TrueInstruction) {
    // Arrange
    chunk.write(Opcode::TRUE, 0);
    chunk.write(Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(Value(true), vm->pop());
}
