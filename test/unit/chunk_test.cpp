#include "../../src/bytecode/chunk.hpp"
#include "../../src/types/value.hpp"
#include "../../src/types/value_formatter.hpp"

#include <gtest/gtest.h>

#include <format>
#include <string>
#include <utility>

class ChunkTest : public ::testing::Test {
  protected:
    Chunk chunk;
    void SetUp() override {
        chunk = Chunk();
    }
};

TEST_F(ChunkTest, AddAndGetConstant) {
    // Arrange
    Value value(42.0);

    // Act
    uint8_t index = chunk.addConstant(value);
    Value result = chunk.getConstant(index);

    // Assert
    EXPECT_EQ(value, result);
}

TEST_F(ChunkTest, GetSize) {
    // Arrange
    chunk.write(Opcode::CONSTANT, 123);
    chunk.write(chunk.addConstant(1.2), 123);

    // Act
    int result = chunk.getSize();

    // Assert
    EXPECT_EQ(result, 2);
}

TEST_F(ChunkTest, GetLine) {
    // Arrange
    chunk.write(Opcode::CONSTANT, 123);

    // Act
    int result = chunk.getLine(0);

    // Assert
    EXPECT_EQ(result, 123);
}

// Test suite for dissassembling simple instructions using parameterized tests
class ChunkParameterizedSimpleInstructionTestFixture : public ::testing::TestWithParam<std::pair<Opcode, std::string>> {
  protected:
    Chunk chunk;
    void SetUp() override {
        chunk = Chunk();
    }
};

// The paramiters for this test suite are a pair of Opcode and the expected string output for that opcode
INSTANTIATE_TEST_SUITE_P(
    ChunkOpCodeDissassemble, ChunkParameterizedSimpleInstructionTestFixture,
    ::testing::Values(std::make_pair(Opcode::ADD, "ADD"), std::make_pair(Opcode::DIVIDE, "DIVIDE"),
                      std::make_pair(Opcode::EQUAL, "EQUAL"), std::make_pair(Opcode::FALSE, "FALSE"),
                      std::make_pair(Opcode::GREATER, "GREATER"),
                      std::make_pair(Opcode::GREATER_EQUAL, "GREATER_EQUAL"), std::make_pair(Opcode::LESS, "LESS"),
                      std::make_pair(Opcode::LESS_EQUAL, "LESS_EQUAL"), std::make_pair(Opcode::MULTIPLY, "MULTIPLY"),
                      std::make_pair(Opcode::NEGATE, "NEGATE"), std::make_pair(Opcode::NOT, "NOT"),
                      std::make_pair(Opcode::NOT_EQUAL, "NOT_EQUAL"), std::make_pair(Opcode::NULL_, "NULL_"),
                      std::make_pair(Opcode::PRINT, "PRINT"), std::make_pair(Opcode::RETURN, "RETURN"),
                      std::make_pair(Opcode::SUBTRACT, "SUBTRACT"), std::make_pair(Opcode::TRUE, "TRUE")));

TEST_P(ChunkParameterizedSimpleInstructionTestFixture, WriteOpCode) {
    auto [opcode, expected] = GetParam();
    chunk.write(opcode, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, std::format("== test chunk ==\n0X0000  123 {:>16}\n", expected));
}

// Test suite for dissassembling constant instructions
class ChunkParameterizedConstantInstructionTestFixture
    : public ::testing::TestWithParam<std::pair<Opcode, std::string>> {
  protected:
    Chunk chunk;
    void SetUp() override {
        chunk = Chunk();
    }
};

INSTANTIATE_TEST_SUITE_P(ChunkOpCodeDissassembleConstantInstruction, ChunkParameterizedConstantInstructionTestFixture,
                         ::testing::Values(std::make_pair(Opcode::CONSTANT, "CONSTANT")));

TEST_P(ChunkParameterizedConstantInstructionTestFixture, WriteOpCode) {
    Value value(1.2);
    auto [opcode, expected] = GetParam();
    chunk.write(opcode, 123);
    chunk.write(chunk.addConstant(value), 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, std::format("== test chunk ==\n0X0000  123 {:>16}{:>16} '{}'\n", expected, 0, value));
}
