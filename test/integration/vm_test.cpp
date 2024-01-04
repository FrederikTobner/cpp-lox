#include "../../src/backend/vm.hpp"
#include "../../src/bytecode/chunk.hpp"
#include "../../src/bytecode/opcode.hpp"
#include "../../src/error/runtime_exception.hpp"
#include "../../src/types/value.hpp"

#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

// Test fixture for VM integration tests
class VMIntegrationTest : public ::testing::Test {
  public:
    std::unique_ptr<cppLox::Backend::VM> vm;
    std::unique_ptr<cppLox::MemoryMutator> memoryMutator;
    cppLox::ByteCode::Chunk chunk;

    VMIntegrationTest() {
        memoryMutator = std::make_unique<cppLox::MemoryMutator>();
        vm = std::make_unique<cppLox::Backend::VM>(memoryMutator.get());
    }

    void SetUp() override {
        vm->resetStack();
        chunk = cppLox::ByteCode::Chunk();
    }
};

TEST_F(VMIntegrationTest, AddInstructionUsingNumbers) {
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
    EXPECT_EQ(cppLox::Types::Value(42.0 + 42.0), vm->pop());
}

TEST_F(VMIntegrationTest, AddInstructionUsingStrings) {
    // Arrange
    auto firstObjectString = std::make_unique<cppLox::Types::ObjectString>("Hello ");
    auto secondObjectString = std::make_unique<cppLox::Types::ObjectString>("World!");
    cppLox::Types::Value value(firstObjectString.get());
    cppLox::Types::Value value2(secondObjectString.get());
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value2), 0);
    chunk.write(cppLox::ByteCode::Opcode::ADD, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);
    char const * result = vm->pop().as<cppLox::Types::Object *>()->as<cppLox::Types::ObjectString>()->string().c_str();

    // Assert
    ASSERT_STREQ("Hello World!", result);
}

TEST_F(VMIntegrationTest, ConstantInstruction) {
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

TEST_F(VMIntegrationTest, DivideInstruction) {
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

TEST_F(VMIntegrationTest, EqualInstruction) {
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

TEST_F(VMIntegrationTest, FalseInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::FALSE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
}

TEST_F(VMIntegrationTest, GreaterInstruction) {
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
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
    // 42 > 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
    // 43 > 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
}

TEST_F(VMIntegrationTest, GreaterEqualInstruction) {
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
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
    // 43 >= 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
    // 42 >= 43
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
}

TEST_F(VMIntegrationTest, LessInstruction) {
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
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
    // 43 < 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
    // 42 < 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
}

TEST_F(VMIntegrationTest, LessEqualInstruction) {
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
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
    // 43 <= 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
    // 42 <= 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
}

TEST_F(VMIntegrationTest, MultiplyInstruction) {
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

TEST_F(VMIntegrationTest, NegateInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value negatedValue(-42.0);
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);
    chunk.write(cppLox::ByteCode::Opcode::NEGATE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    ASSERT_EQ(negatedValue, vm->pop());
}

TEST_F(VMIntegrationTest, NotEqualInstruction) {
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
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop());
    // 42 != 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop());
}

TEST_F(VMIntegrationTest, NullInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::NULL_, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    EXPECT_EQ(cppLox::Types::Value(), vm->pop());
}

TEST_F(VMIntegrationTest, PushAndPop) {
    // Arrange
    cppLox::Types::Value value(42.0);

    // Act
    vm->push(value);
    cppLox::Types::Value result = vm->pop();

    // Assert
    ASSERT_EQ(value, result);
}

TEST_F(VMIntegrationTest, ReturnInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act & Assert
    ASSERT_NO_THROW(vm->interpret(chunk));
}

TEST_F(VMIntegrationTest, StackOverflow) {
    // Arrange
    cppLox::Types::Value value(42.0);
    for (size_t i : std::ranges::iota_view(0, 256)) {
        vm->push(value);
    }
    cppLox::ByteCode::Chunk chunk;
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(value), 0);

    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), cppLox::Error::RunTimeException);
}

TEST_F(VMIntegrationTest, StackUnderflow) {
    cppLox::ByteCode::Chunk chunk;
    chunk.write(cppLox::ByteCode::Opcode::ADD, 0);
    // Act & Assert
    ASSERT_THROW(vm->interpret(chunk), cppLox::Error::RunTimeException);
}

TEST_F(VMIntegrationTest, SubtractInstruction) {
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
    ASSERT_EQ(value - value, vm->pop());
}

TEST_F(VMIntegrationTest, VariableDeclaration) {
    // Arrange
    auto variableNameObjectString = std::make_unique<cppLox::Types::ObjectString>("a");
    chunk.write(cppLox::ByteCode::Opcode::NULL_, 0);
    chunk.write(cppLox::ByteCode::Opcode::DEFINE_GLOBAL, 0);
    chunk.write(chunk.addConstant(cppLox::Types::Value(variableNameObjectString.get())), 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    ASSERT_TRUE(memoryMutator->setGlobal(variableNameObjectString.get(), cppLox::Types::Value()));
}

TEST_F(VMIntegrationTest, VariableDeclarationAndAssignment) {
    // Arrange
    auto value = std::make_unique<cppLox::Types::Value>(42.0);
    auto variableNameObjectString = std::make_unique<cppLox::Types::ObjectString>("a");
    chunk.write(cppLox::ByteCode::Opcode::CONSTANT, 0);
    chunk.write(chunk.addConstant(*value), 0);
    chunk.write(cppLox::ByteCode::Opcode::DEFINE_GLOBAL, 0);
    chunk.write(chunk.addConstant(cppLox::Types::Value(variableNameObjectString.get())), 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    ASSERT_EQ(cppLox::Types::Value(42.0), memoryMutator->getGlobal(variableNameObjectString.get()));
}

TEST_F(VMIntegrationTest, TrueInstruction) {
    // Arrange
    chunk.write(cppLox::ByteCode::Opcode::TRUE, 0);
    chunk.write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act
    vm->interpret(chunk);

    // Assert
    ASSERT_EQ(cppLox::Types::Value(true), vm->pop());
}
