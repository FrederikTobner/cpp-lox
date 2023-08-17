#include "../../src/chunk.hpp"
#include "../../src/value.hpp"

#include <gtest/gtest.h>

#include <format>
#include <string>
#include <tuple>
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
    chunk.write(OP_CONSTANT, 123);
    chunk.write(chunk.addConstant(1.2), 123);

    // Act
    int result = chunk.getSize();

    // Assert
    EXPECT_EQ(result, 2);
}

TEST_F(ChunkTest, GetLine) {
    // Arrange
    chunk.write(OP_CONSTANT, 123);

    // Act
    int result = chunk.getLine(0);

    // Assert
    EXPECT_EQ(result, 123);
}

// Test suite for dissassembling simple instructions
class ChunkParameterizedSimpleInstructionTestFixture : public ::testing::TestWithParam<std::pair<Opcode, std::string>> {
  protected:
    Chunk chunk;
    void SetUp() override {
        chunk = Chunk();
    }
};

INSTANTIATE_TEST_SUITE_P(
    ChunkOpCodeDissassemble, ChunkParameterizedSimpleInstructionTestFixture,
    ::testing::Values(std::make_pair(OP_ADD, "OP_ADD"), std::make_pair(OP_DIVIDE, "OP_DIVIDE"),
                      std::make_pair(OP_EQUAL, "OP_EQUAL"), std::make_pair(OP_FALSE, "OP_FALSE"),
                      std::make_pair(OP_GREATER, "OP_GREATER"), std::make_pair(OP_GREATER_EQUAL, "OP_GREATER_EQUAL"),
                      std::make_pair(OP_LESS, "OP_LESS"), std::make_pair(OP_LESS_EQUAL, "OP_LESS_EQUAL"),
                      std::make_pair(OP_MULTIPLY, "OP_MULTIPLY"), std::make_pair(OP_NEGATE, "OP_NEGATE"),
                      std::make_pair(OP_NOT, "OP_NOT"), std::make_pair(OP_NOT_EQUAL, "OP_NOT_EQUAL"),
                      std::make_pair(OP_NULL, "OP_NULL"), std::make_pair(OP_PRINT, "OP_PRINT"),
                      std::make_pair(OP_RETURN, "OP_RETURN"), std::make_pair(OP_SUBTRACT, "OP_SUBTRACT"),
                      std::make_pair(OP_TRUE, "OP_TRUE")));

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
                         ::testing::Values(std::make_pair(OP_CONSTANT, "OP_CONSTANT")));

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
