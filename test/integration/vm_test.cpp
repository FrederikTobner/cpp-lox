#include "../../src/backend/vm.hpp"
#include "../../src/bytecode/chunk.hpp"
#include "../../src/bytecode/opcode.hpp"
#include "../../src/error/runtime_exception.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/types/value.hpp"

#include <iostream>
#include <ranges>
#include <variant>

#include <gtest/gtest.h>

// Test fixture for VM integration tests
class VMIntegrationTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Backend::VM> vm;
    std::shared_ptr<cppLox::MemoryMutator> memoryMutator;
    cppLox::Types::ObjectFunction function;
    cppLox::Types::ObjectString functionName = cppLox::Types::ObjectString("test");

    VMIntegrationTest() : function(0, &functionName) {
        memoryMutator = std::make_shared<cppLox::MemoryMutator>();
        vm = std::make_unique<cppLox::Backend::VM>(memoryMutator);
    }

    void SetUp() override {
        vm->resetStack();
    }

    template <class... Opcodes>
        requires cppLox::Frontend::IsPackOfEitherOf<size_t, cppLox::ByteCode::Opcode, Opcodes...>
    void writeMultipleToChunk(Opcodes... expectedOpCodes) {
        constexpr size_t amountOfOpcodesToWrite = sizeof...(expectedOpCodes);
        std::array<std::variant<size_t, cppLox::ByteCode::Opcode>, amountOfOpcodesToWrite> opcodes = {
            expectedOpCodes...};
        for (auto index : std::views::iota(0u, amountOfOpcodesToWrite)) {
            std::visit([this, index](auto && arg) { function.chunk()->write(arg, 0); }, opcodes[index]);
        }
    }

    cppLox::Backend::CallFrame & getTopFrame() {
        return vm->m_frames[0];
    }

    void interpret() {
        vm->interpret(function);
        // The value on top of the stack is popped when the function returns,
        // so we need to increment the stack top to get the value that was on top of the stack
        vm->m_stack_top++;
    }
};

TEST_F(VMIntegrationTest, AddInstructionUsingNumbers) {
    // Arrange
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::ADD, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    //  Assert
    EXPECT_EQ(cppLox::Types::Value(42.0 + 42.0), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, AddInstructionUsingStrings) {
    // Arrange
    auto firstObjectString = std::make_unique<cppLox::Types::ObjectString>("Hello ");
    auto secondObjectString = std::make_unique<cppLox::Types::ObjectString>("World!");
    cppLox::Types::Value value(firstObjectString.get());
    cppLox::Types::Value value2(secondObjectString.get());
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
                         cppLox::ByteCode::Opcode::ADD, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();
    char const * result =
        vm->pop(getTopFrame()).as<cppLox::Types::Object *>()->as<cppLox::Types::ObjectString>()->string().c_str();

    // Assert
    ASSERT_STREQ("Hello World!", result);
}

TEST_F(VMIntegrationTest, ConstantInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    EXPECT_EQ(value, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, DivideInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::DIVIDE, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    EXPECT_EQ(value / value, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, EqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    cppLox::Types::Value value3(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
                         // 42 == 43
                         cppLox::ByteCode::Opcode::EQUAL, cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(value3), cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(value),
                         // 42 == 42
                         cppLox::ByteCode::Opcode::EQUAL, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 == 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
    // 42 == 43
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, FalseInstruction) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::FALSE, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, GlobalVariableDeclaration) {
    // Arrange
    auto variableNameObjectString = std::make_unique<cppLox::Types::ObjectString>("a");
    writeMultipleToChunk(cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::DEFINE_GLOBAL,
                         function.chunk()->addConstant(cppLox::Types::Value(variableNameObjectString.get())),
                         cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_TRUE(memoryMutator->setGlobal(variableNameObjectString.get(), cppLox::Types::Value()));
}

TEST_F(VMIntegrationTest, GlobalVariableDeclarationAndAssignment) {
    // Arrange
    cppLox::Types::Value value(42.0);
    auto variableNameObjectString = std::make_unique<cppLox::Types::ObjectString>("a");
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::DEFINE_GLOBAL,
                         function.chunk()->addConstant(cppLox::Types::Value(variableNameObjectString.get())),
                         cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(cppLox::Types::Value(42.0), memoryMutator->getGlobal(variableNameObjectString.get()));
}

TEST_F(VMIntegrationTest, GreaterInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    writeMultipleToChunk(
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value), cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value2),
        // 42 > 43
        cppLox::ByteCode::Opcode::GREATER, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 43 > 42
        cppLox::ByteCode::Opcode::GREATER, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 42 > 42
        cppLox::ByteCode::Opcode::GREATER, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 > 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
    // 42 > 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
    // 43 > 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, GreaterEqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    writeMultipleToChunk(
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value), cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value2),
        // 42 >= 43
        cppLox::ByteCode::Opcode::GREATER_EQUAL, cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value2), cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 43 >= 42
        cppLox::ByteCode::Opcode::GREATER_EQUAL, cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value), cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 42 >= 42
        cppLox::ByteCode::Opcode::GREATER_EQUAL, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 >= 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
    // 43 >= 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
    // 42 >= 43
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, JumpIfFalseInstruction_shouldJumpWhenFalsy) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::FALSE, cppLox::ByteCode::Opcode::JUMP_IF_FALSE, (size_t)0x00,
                         (size_t)0x02, cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(cppLox::Types::Value(42.0)), cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, JumpIfFalseInstruction_shouldNotJumpWhenTruthy) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::JUMP_IF_FALSE, (size_t)0x00,
                         (size_t)0x02, cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(cppLox::Types::Value(42.0)), cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(cppLox::Types::Value(42.0), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, JumpInstruction) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(cppLox::Types::Value(42.0)),
                         cppLox::ByteCode::Opcode::JUMP, (size_t)0x00, (size_t)0x02, cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(cppLox::Types::Value(43.0)), cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(cppLox::Types::Value(42.0), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, LessInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    writeMultipleToChunk(
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value), cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value2),
        // 42 < 43
        cppLox::ByteCode::Opcode::LESS, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 43 < 42
        cppLox::ByteCode::Opcode::LESS, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 42 < 42
        cppLox::ByteCode::Opcode::LESS, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 < 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
    // 43 < 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
    // 42 < 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, LessEqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    writeMultipleToChunk(
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value), cppLox::ByteCode::Opcode::CONSTANT,
        function.chunk()->addConstant(value2),
        // 42 <= 43
        cppLox::ByteCode::Opcode::LESS_EQUAL, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 43 <= 42
        cppLox::ByteCode::Opcode::LESS_EQUAL, cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
        // 42 <= 42
        cppLox::ByteCode::Opcode::LESS_EQUAL, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 <= 42
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
    // 43 <= 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
    // 42 <= 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, LocalVariableAssignement) {
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(value, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, MultiplyInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::MULTIPLY, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    EXPECT_EQ(value * value, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, NegateInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value negatedValue(-42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::NEGATE, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(negatedValue, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, NotEqualInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    cppLox::Types::Value value2(43.0);
    cppLox::Types::Value value3(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value2),
                         // 42 != 43
                         cppLox::ByteCode::Opcode::NOT_EQUAL, cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(value3), cppLox::ByteCode::Opcode::CONSTANT,
                         function.chunk()->addConstant(value),
                         // 42 != 42
                         cppLox::ByteCode::Opcode::NOT_EQUAL, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    // 42 != 42
    EXPECT_EQ(cppLox::Types::Value(false), vm->pop(getTopFrame()));
    // 42 != 43
    EXPECT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, NullInstruction) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    EXPECT_EQ(cppLox::Types::Value(), vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, PushAndPop) {
    // Arrange
    cppLox::Types::Value value(42.0);

    // Act
    vm->push(getTopFrame(), value);
    cppLox::Types::Value result = vm->pop(getTopFrame());

    // Assert
    ASSERT_EQ(value, result);
}

TEST_F(VMIntegrationTest, ReturnInstruction) {
    // Arrange
    function.chunk()->write(cppLox::ByteCode::Opcode::RETURN, 0);

    // Act & Assert
    ASSERT_NO_THROW(vm->interpret(function));
}

TEST_F(VMIntegrationTest, StackOverflow) {
    // Arrange
    cppLox::Types::Value value(42.0);
    for (auto i : std::views::iota(0, STACK_MAX - 1)) {
        vm->push(getTopFrame(), value);
    }
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value));

    // Act & Assert
    ASSERT_THROW(vm->interpret(function), cppLox::Error::RunTimeException);
}

TEST_F(VMIntegrationTest, StackUnderflow) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::ADD);

    // Act & Assert
    ASSERT_THROW(vm->interpret(function), cppLox::Error::RunTimeException);
}

TEST_F(VMIntegrationTest, SubtractInstruction) {
    // Arrange
    cppLox::Types::Value value(42.0);
    writeMultipleToChunk(cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::CONSTANT, function.chunk()->addConstant(value),
                         cppLox::ByteCode::Opcode::SUBTRACT, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(value - value, vm->pop(getTopFrame()));
}

TEST_F(VMIntegrationTest, TrueInstruction) {
    // Arrange
    writeMultipleToChunk(cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::RETURN);

    // Act
    interpret();

    // Assert
    ASSERT_EQ(cppLox::Types::Value(true), vm->pop(getTopFrame()));
}
