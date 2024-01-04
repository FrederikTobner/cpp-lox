#include <gtest/gtest.h>

#include <array>
#include <ranges>
#include <vector>

#include "../../src/bytecode/opcode.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/frontend/token.hpp"
#include "../../src/memory_mutator.hpp"

// Test fixture for Compiler integration tests
class CompilerIntegrationTest : public ::testing::Test {
  protected:
    std::unique_ptr<cppLox::Frontend::Compiler> compiler;
    std::unique_ptr<cppLox::MemoryMutator> memoryMutator;
    std::vector<cppLox::Frontend::Token> tokens;
    std::unique_ptr<cppLox::ByteCode::Chunk> chunky;

    CompilerIntegrationTest() {
        memoryMutator = std::make_unique<cppLox::MemoryMutator>();
        compiler = std::make_unique<cppLox::Frontend::Compiler>(memoryMutator.get());
    }

    void SetUp() override {
        tokens = std::vector<cppLox::Frontend::Token>();
        chunky = nullptr;
    }

    class OpcodeContainer {
        uint8_t opcode;

      public:
        OpcodeContainer(int opcode) : opcode(opcode) {
        }

        OpcodeContainer(uint8_t opcode) : opcode(opcode) {
        }

        OpcodeContainer(cppLox::ByteCode::Opcode opcode) : opcode(static_cast<uint8_t>(opcode)) {
        }

        uint8_t getOpCode() const {
            return opcode;
        }
    };

    template <class... Opcodes>
        requires(std::is_convertible_v<Opcodes, OpcodeContainer> && ...)
    void assertChunkContaintsExactlyInOrder(Opcodes... expectedOpCodes) {
        constexpr size_t expectedOpCodeAmount = sizeof...(expectedOpCodes);
        ASSERT_EQ(expectedOpCodeAmount, chunky->getSize());
        std::array<OpcodeContainer, expectedOpCodeAmount> opcodes = {expectedOpCodes...};
        for (const size_t i : std::views::iota(0u, expectedOpCodeAmount)) {
            ASSERT_EQ(opcodes[i].getOpCode(), chunky->getByte(i));
        }
    }
};

TEST_F(CompilerIntegrationTest, AdditionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    chunky = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::ADD, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, DivisionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SLASH, "/", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    chunky = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::DIVIDE, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, EqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL_EQUAL, "==", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, FalseLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::FALSE, "false", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 3);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::FALSE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, GreaterThanExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER, ">", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::GREATER));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, GreaterThanEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER_EQUAL, ">=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::GREATER_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, GroupingExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "3", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 10);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::ADD));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(6), 2);
    ASSERT_EQ(chunk->getByte(7), static_cast<uint8_t>(cppLox::ByteCode::Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(8), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(9), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, LessThanExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS, "<", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::LESS));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, LessThanEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS_EQUAL, "<=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::LESS_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, MultiplicationExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::MULTIPLY));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, NegateExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 5);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NEGATE));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, NotExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG, "!", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 5);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NOT));
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, NotEqualExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG_EQUAL, "!=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NOT_EQUAL));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, NullLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NULL_, "null", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 3);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NULL_));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, SubtractionExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 7);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(1), 0);
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT));
    ASSERT_EQ(chunk->getByte(3), 1);
    ASSERT_EQ(chunk->getByte(4), static_cast<uint8_t>(cppLox::ByteCode::Opcode::SUBTRACT));
    ASSERT_EQ(chunk->getByte(5), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(6), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, TrueLiteralExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 3);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::TRUE));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP));
    ASSERT_EQ(chunk->getByte(2), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, VariableDeclarationExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(chunk->getSize(), 4);
    ASSERT_EQ(chunk->getByte(0), static_cast<uint8_t>(cppLox::ByteCode::Opcode::NULL_));
    ASSERT_EQ(chunk->getByte(1), static_cast<uint8_t>(cppLox::ByteCode::Opcode::DEFINE_GLOBAL));
    ASSERT_EQ(chunk->getByte(2), 0);
    ASSERT_EQ(chunk->getByte(3), static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN));
}

TEST_F(CompilerIntegrationTest, VariableAssignmentExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(6, chunk->getSize());
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT), chunk->getByte(0));
    ASSERT_EQ(1, chunk->getByte(1));
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::SET_GLOBAL), chunk->getByte(2));
    ASSERT_EQ(0, chunk->getByte(3));
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::POP), chunk->getByte(4));
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN), chunk->getByte(5));
}

TEST_F(CompilerIntegrationTest, VariableDeclarationAndAssignmentExpression) {
    // Arrange
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1));
    tokens.push_back(cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1));

    // Act
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler->compile(tokens);

    // Assert
    ASSERT_EQ(5, chunk->getSize());
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::CONSTANT), chunk->getByte(0));
    ASSERT_EQ(1, chunk->getByte(1));
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::DEFINE_GLOBAL), chunk->getByte(2));
    ASSERT_EQ(0, chunk->getByte(3));
    ASSERT_EQ(static_cast<uint8_t>(cppLox::ByteCode::Opcode::RETURN), chunk->getByte(4));
}