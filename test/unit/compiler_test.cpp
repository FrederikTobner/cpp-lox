#include "../../src/compiler.hpp"

#include <gtest/gtest.h>

#include <vector>

#include "../../src/memory_manager.hpp"
#include "../../src/opcode.hpp"
#include "../../src/token.hpp"


// Test fixture for Compiler integration tests
class CompilerTest : public ::testing::Test {
  public:
    std::unique_ptr<Compiler> compiler;
    std::unique_ptr<MemoryManager> memory_manager;
    std::vector<Token> tokens;

    CompilerTest() {
        memory_manager = std::make_unique<MemoryManager>();
        compiler = std::make_unique<Compiler>(memory_manager.get());
    }

    void SetUp() override {
        tokens = std::vector<Token>();
    }
};

TEST_F(CompilerTest, AdditionExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::PLUS, "+", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_ADD);
    ASSERT_EQ(chunk->getByte(5), OP_RETURN);
}

TEST_F(CompilerTest, SubtractionExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::MINUS, "-", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_SUBTRACT);
    ASSERT_EQ(chunk->getByte(5), OP_RETURN);
}

TEST_F(CompilerTest, MultiplicationExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::STAR, "*", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_MULTIPLY);
    ASSERT_EQ(chunk->getByte(5), OP_RETURN);
}

TEST_F(CompilerTest, DivisionExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::SLASH, "/", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_DIVIDE);
    ASSERT_EQ(chunk->getByte(5), OP_RETURN);
}

TEST_F(CompilerTest, NegateExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::MINUS, "-", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 4);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_NEGATE);
    ASSERT_EQ(chunk->getByte(3), OP_RETURN);
}

TEST_F(CompilerTest, GroupingExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::LEFT_PARENTHESES, "(", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::PLUS, "+", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::RIGHT_PARENTHESES, ")", 1));
    tokens.push_back(Token(Token::Type::STAR, "*", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "3", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 9);
    ASSERT_EQ(chunk->getByte(0), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), OP_ADD);
    ASSERT_EQ(chunk->getByte(5), OP_CONSTANT);
    ASSERT_EQ(chunk->getByte(6), 2);
    ASSERT_EQ(chunk->getByte(7), OP_MULTIPLY);
    ASSERT_EQ(chunk->getByte(8), OP_RETURN);
}