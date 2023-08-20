#include <gtest/gtest.h>

#include <vector>

#include "../../src/bytecode/opcode.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/frontend/token.hpp"
#include "../../src/memory_mutator.hpp"

// Test fixture for Compiler integration tests
class CompilerTest : public ::testing::Test {
  public:
    std::unique_ptr<Compiler> compiler;
    std::unique_ptr<MemoryMutator> memoryMutator;
    std::vector<Token> tokens;

    CompilerTest() {
        memoryMutator = std::make_unique<MemoryMutator>();
        compiler = std::make_unique<Compiler>(memoryMutator.get());
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::ADD));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::SUBTRACT));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::DIVIDE));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, EqualExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::EQUAL_EQUAL, "==", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, FalseLiteralExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::FALSE, "false", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::FALSE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, GreaterThanExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::GREATER, ">", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::GREATER));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, GreaterThanEqualExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::GREATER_EQUAL, ">=", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::GREATER_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::ADD));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(6), 2);
    ASSERT_EQ(chunk->getByte(7), static_cast<uint8_t>(Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(8), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, LessThanExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::LESS, "<", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::LESS));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, LessThanEqualExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::LESS_EQUAL, "<=", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));
    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::LESS_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
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
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::NEGATE));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, NotExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::BANG, "!", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));
    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 4);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::NOT));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, NotEqualExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NUMBER, "1", 1));
    tokens.push_back(Token(Token::Type::BANG_EQUAL, "!=", 1));
    tokens.push_back(Token(Token::Type::NUMBER, "2", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(Opcode::NOT_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, NullLiteralExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::NULL_, "null", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::NULL_));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(Opcode::RETURN));
}

TEST_F(CompilerTest, TrueLiteralExpression) {
    // Arrange
    tokens.push_back(Token(Token::Type::TRUE, "true", 1));
    tokens.push_back(Token(Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(Opcode::TRUE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(Opcode::RETURN));
}