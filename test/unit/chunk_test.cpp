#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/value.hpp"
#include "gtest/gtest.h"

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

TEST_F(ChunkTest, Disassemble) {
    // Arrange
    chunk.write(OP_CONSTANT, 123);
    chunk.write(chunk.addConstant(1.2), 123);
    chunk.write(OP_RETURN, 123);
    testing::internal::CaptureStdout();

    // Act
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    // Assert
    EXPECT_EQ(output,
              "== test chunk ==\n0000  123      OP_CONSTANT               0 '1.2'\n0002    |        OP_RETURN\n");
}
