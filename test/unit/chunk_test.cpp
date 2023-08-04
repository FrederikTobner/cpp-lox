#include "../../src/chunk.hpp"
#include "../../src/opcode.hpp"
#include "../../src/value.hpp"
#include "gtest/gtest.h"

TEST(ChunkTest, WriteAndDisassemble) {
    Chunk chunk;
    chunk.write(OP_CONSTANT, 123);
    chunk.write(chunk.addConstant(1.2), 123);
    chunk.write(OP_RETURN, 123);

    testing::internal::CaptureStdout();
    chunk.disassemble("test chunk");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output,
              "== test chunk ==\n0000  123      OP_CONSTANT               0 '1.2'\n0002    |        OP_RETURN\n");
}

TEST(ChunkTest, DisassembleEmptyChunk) {
    Chunk chunk;

    testing::internal::CaptureStdout();
    chunk.disassemble("empty chunk");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "== empty chunk ==\n");
}

TEST(ChunkTest, DisassembleSingleInstruction) {
    Chunk chunk;
    chunk.write(OP_RETURN, 123);

    testing::internal::CaptureStdout();
    chunk.disassemble("single instruction chunk");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "== single instruction chunk ==\n0000  123        OP_RETURN\n");
}