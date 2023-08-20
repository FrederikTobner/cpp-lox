#include <gtest/gtest.h>

#include <vector>

#include "../../src/bytecode/opcode.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/frontend/token.hpp"
#include "../../src/memory_mutator.hpp"

// Test fixture for Compiler integration tests
class CompilerTest : public ::testing::Test {
  public:
    std::unique_ptr<cppLox::Frontend::Compiler> compiler;
    std::unique_ptr<MemoryMutator> memoryMutator;
    std::vector<cppLox::Frontend::Token> tokens;

    CompilerTest() {
        memoryMutator = std::make_unique<MemoryMutator>();
        compiler = std::make_unique<cppLox::Frontend::Compiler>(memoryMutator.get());
    }

    void SetUp() override {
        tokens = std::vector<cppLox::Frontend::Token>();
    }
};

TEST_F(CompilerTest, AdditionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::ADD));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, SubtractionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::SUBTRACT));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, MultiplicationExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, DivisionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SLASH, "/", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::DIVIDE));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, EqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL_EQUAL, "==", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, FalseLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::FALSE, "false", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::FALSE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, GreaterThanExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER, ">", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::GREATER));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, GreaterThanEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER_EQUAL, ">=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::GREATER_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, GroupingExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "3", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 9);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::ADD));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(6), 2);
    ASSERT_EQ(chunk->getByte(7), static_cast<uint8_t>(cppLox::ByteCode::Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(8), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, LessThanExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS, "<", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::LESS));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, LessThanEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS_EQUAL, "<=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));
    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::LESS_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, NegateExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 4);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NEGATE));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, NotExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG, "!", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));
    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 4);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NOT));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, NotEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG_EQUAL, "!=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 6);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NOT_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, NullLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NULL_, "null", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NULL_));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerTest, TrueLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);
    // Assert
    ASSERT_EQ(chunk->getSize(), 2);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::TRUE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}