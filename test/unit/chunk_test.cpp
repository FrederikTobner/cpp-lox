#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/value.hpp"

#include <gtest/gtest.h>

class ChunkTest : public ::testing::Test {
  public:
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

TEST_F(ChunkTest, DisassembleConstant) {
    // Arrange
    chunk.write(OP_CONSTANT, 123);
    chunk.write(chunk.addConstant(1.2), 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123      OP_CONSTANT               0 '1.2'\n");
}

TEST_F(ChunkTest, DisassembleReturn) {
    // Arrange
    chunk.write(OP_RETURN, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123        OP_RETURN\n");
}

TEST_F(ChunkTest, DisassembleAdd) {
    // Arrange
    chunk.write(OP_ADD, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123           OP_ADD\n");
}

TEST_F(ChunkTest, DisassembleSubtract) {
    // Arrange
    chunk.write(OP_SUBTRACT, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123      OP_SUBTRACT\n");
}

TEST_F(ChunkTest, DisassembleMultiply) {
    // Arrange
    chunk.write(OP_MULTIPLY, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123      OP_MULTIPLY\n");
}

TEST_F(ChunkTest, DisassembleDivide) {
    // Arrange
    chunk.write(OP_DIVIDE, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output, "== test chunk ==\n0000  123        OP_DIVIDE\n");
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