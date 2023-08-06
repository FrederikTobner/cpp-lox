#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/value.hpp"
#include "../../src/vm.hpp"

#include <gtest/gtest.h>

// Test fixture for VM integration tests
class VMTestIT : public ::testing::Test {
  public:
    VM vm;
    Chunk chunk;
    void SetUp() override {
        chunk = Chunk();
        vm = VM();
    }
};

TEST_F(VMTestIT, AddInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_ADD, 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(value + value, vm.pop());
}

TEST_F(VMTestIT, ConstantInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);
    Value valueFromStack = vm.pop();

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(value, valueFromStack);
}

TEST_F(VMTestIT, DivideInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_DIVIDE, 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(value / value, vm.pop());
}

TEST_F(VMTestIT, MultiplyInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_MULTIPLY, 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(value * value, vm.pop());
}

TEST_F(VMTestIT, NegateInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_NEGATE, 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(-value, vm.pop());
}

TEST_F(VMTestIT, ReturnInstruction) {
    // Arrange
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
}

TEST_F(VMTestIT, SubtractInstruction) {
    // Arrange
    Value value(42.0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(OP_SUBTRACT, 0);
    chunk.write(OP_RETURN, 0);

    // Act
    VM::InterpretResult interpretResult = vm.interpret(chunk);

    // Assert
    EXPECT_EQ(VM::InterpretResult::INTERPRET_OK, interpretResult);
    EXPECT_EQ(value - value, vm.pop());
}