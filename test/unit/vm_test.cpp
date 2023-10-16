#include "../../src/backend/vm.hpp"
#include "../../src/bytecode/chunk.hpp"
#include "../../src/bytecode/opcode.hpp"
#include "../../src/error/runtime_exception.hpp"
#include "../../src/types/value.hpp"

#include <iostream>

#include <gtest/gtest.h>

// Test fixture for VM unit tests
class VMTest : public ::testing::Test {
  public:
    std::unique_ptr<cppLox::Backend::VM> vm;
    std::unique_ptr<MemoryMutator> memoryMutator;
    cppLox::ByteCode::Chunk chunk;

    VMTest() {
        memoryMutator = std::make_unique<MemoryMutator>();
        vm = std::make_unique<cppLox::Backend::VM>(memoryMutator.get());
    }

    void SetUp() override {
        vm->resetStack();
        chunk = cppLox::ByteCode::Chunk();
    }
};

TEST_F(VMTest, AddInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::ADD, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value + value, vm->pop());
}

TEST_F(VMTest, ConstantInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value, vm->pop());
}

TEST_F(VMTest, DivideInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::DIVIDE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value / value, vm->pop());
}

TEST_F(VMTest, EqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    cppLox::Types::Value value3(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 == 43
    chunk.write(cppLox::ByteCode::Opcode::EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value3), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 == 42
    chunk.write(cppLox::ByteCode::Opcode::EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    // 42 == 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
    // 42 == 43
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
}

TEST_F(VMTest, FalseInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::FALSE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
}

TEST_F(VMTest, GreaterInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 > 43
    chunk.write(cppLox::ByteCode::Opcode::GREATER, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 43 > 42
    chunk.write(cppLox::ByteCode::Opcode::GREATER, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 > 42
    chunk.write(cppLox::ByteCode::Opcode::GREATER, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

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
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 43 >= 42
    chunk.write(cppLox::ByteCode::Opcode::GREATER_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 >= 43
    chunk.write(cppLox::ByteCode::Opcode::GREATER_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 >= 42
    chunk.write(cppLox::ByteCode::Opcode::GREATER_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

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
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 < 43
    chunk.write(cppLox::ByteCode::Opcode::LESS, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 43 < 42
    chunk.write(cppLox::ByteCode::Opcode::LESS, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 < 42
    chunk.write(cppLox::ByteCode::Opcode::LESS, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

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
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 43 <= 42
    chunk.write(cppLox::ByteCode::Opcode::LESS_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 <= 43
    chunk.write(cppLox::ByteCode::Opcode::LESS_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 <= 42
    chunk.write(cppLox::ByteCode::Opcode::LESS_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

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
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::MULTIPLY, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value * value, vm->pop());
}

TEST_F(VMTest, NegateInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::NEGATE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(-value, vm->pop());
}

TEST_F(VMTest, NotEqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    cppLox::Types::Value value3(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    // 42 != 43
    chunk.write(cppLox::ByteCode::Opcode::NOT_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value3), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // 42 != 42
    chunk.write(cppLox::ByteCode::Opcode::NOT_EQUAL, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

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
    chunk.write(cppLox::ByteCode::Opcode::NULL_, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(cppLox::Types::Value(), vm->pop());
}

TEST_F(VMTest, PushAndPop) {
    // Arrange
    cppLox::Types::Value value(42.0);

    // Act
    vm->push(value);
    cppLox::Types::Value result = vm->pop();

    // Assert
    ASSERT_EQ(value, result);
}

TEST_F(VMTest, ReturnInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act & Assert
    ASSERT_NO_THROW(vm->interpret(chunk));
}

TEST_F(VMTest, StackOverflow) {
    // Arrange
    cppLox::Types::Value value(42.0);
    for (int i = 0; i < 256; i++) {
        vm->push(value);
    }
    cppLox::ByteCode::Chunk chunk;
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), cppLox::Error::RunTimeException);
}

TEST_F(VMTest, StackUnderflow) {
    cppLox::ByteCode::Chunk chunk;
    chunk.write(cppLox::ByteCode::Opcode::ADD, 0);
    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), cppLox::Error::RunTimeException);
}

TEST_F(VMTest, SubtractInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::SUBTRACT, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(value - value, vm->pop());
}

TEST_F(VMTest, TrueInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::TRUE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
}
