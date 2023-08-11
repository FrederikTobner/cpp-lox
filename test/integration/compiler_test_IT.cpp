#include "../../src/compiler.hpp"

#include <gtest/gtest.h>

#include <vector>

#include "../../src/opcode.hpp"
#include "../../src/token.hpp"

// Test fixture for Compiler integration tests
class CompilerTestIT : public ::testing::Test {
  public:
    Compiler compiler;
    void SetUp() override {
        compiler = Compiler();
    }
};

TEST_F(CompilerTestIT, CanCreateCompiler) {
    // Arrange
    std::vector<Token> tokens;
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::PLUS, "+", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler.compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_ADD);
    ASSERT_EQ(chunk->getByte(5), OP_RETURN);
}