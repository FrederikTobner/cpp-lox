#include <gtest/gtest.h>

#include <array>
#include <format>
#include <ranges>
#include <variant>
#include <vector>

#include "../../src/bytecode/opcode.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/frontend/token.hpp"
#include "../../src/memory_mutator.hpp"
#include "../../src/types/object_function.hpp"

// Test fixture for Compiler integration tests
class CompilerIntegrationTest : public ::testing::Test {
  protected:
    CompilerIntegrationTest() {
        memoryMutator = std::make_shared<cppLox::MemoryMutator>();
        compiler = std::make_unique<cppLox::Frontend::Compiler>(memoryMutator);
    }

    void SetUp() override {
        tokens.clear();
        objectFunction = std::nullopt;
    }

    template <class... Opcodes>
        requires cppLox::Frontend::IsPackOfEitherOf<int, cppLox::ByteCode::Opcode, Opcodes...>
    auto assertChunkContaintsExactlyInOrder(Opcodes... expectedOpCodes) -> void {
        ASSERT_TRUE(objectFunction.has_value()) << "Expected compiled function to have a value but it was empty.";
        cppLox::ByteCode::Chunk * chunk = objectFunction.value()->chunk();
        constexpr size_t expectedOpCodeAmount = sizeof...(expectedOpCodes);
        ASSERT_EQ(expectedOpCodeAmount, chunk->getSize());
        std::array<std::variant<int, cppLox::ByteCode::Opcode>, expectedOpCodeAmount> opcodes = {expectedOpCodes...};
        for (auto index : std::views::iota(0u, expectedOpCodeAmount)) {
            std::visit(
                [chunk, index](auto && arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, cppLox::ByteCode::Opcode>) {
                        EXPECT_EQ(arg, chunk->getByte(index))
                            << "Expected " << cppLox::ByteCode::opcode_as_string(arg) << " at index " << index
                            << " but got "
                            << (chunk->getByte(index) < cppLox::ByteCode::AMOUNT
                                    ? cppLox::ByteCode::opcode_as_string(
                                          static_cast<cppLox::ByteCode::Opcode>(chunk->getByte(index)))
                                    : std::format("{}", chunk->getByte(index)))
                            << " instead.";
                    } else {
                        EXPECT_EQ(arg, chunk->getByte(index)) << "Expected " << arg << " at index " << index
                                                              << " but got " << chunk->getByte(index) << " instead.";
                    }
                },
                opcodes[index]);
        }
    }

    auto getObjectAt(size_t index) -> cppLox::Types::Object * {
        return memoryMutator->m_objects[index].get();
    }

    std::unique_ptr<cppLox::Frontend::Compiler> compiler;
    std::shared_ptr<cppLox::MemoryMutator> memoryMutator;
    std::vector<cppLox::Frontend::Token> tokens;
    std::optional<cppLox::Types::ObjectFunction *> objectFunction;
};

TEST_F(CompilerIntegrationTest, AdditionExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::ADD, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, CallExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::GET_GLOBAL, 0, cppLox::ByteCode::Opcode::CALL, 0,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::NULL_,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, FunctionDefinition) {
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::FUN, "fun", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_BRACE, "{", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_BRACE, "}", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    cppLox::Types::ObjectString functionName("a");
    auto functionObject = getObjectAt(3)->as<cppLox::Types::ObjectFunction>();
    ASSERT_EQ(functionObject->arity(), 0);
    ASSERT_EQ(functionObject->name()->string(), "a");
    ASSERT_EQ(functionObject->chunk()->getSize(), 2);
    ASSERT_EQ(functionObject->chunk()->getByte(0), cppLox::ByteCode::Opcode::NULL_);
    ASSERT_EQ(functionObject->chunk()->getByte(1), cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, DivisionExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SLASH, "/", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::DIVIDE, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, EqualExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL_EQUAL, "==", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::EQUAL, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, FalseLiteralExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::FALSE, "false", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::FALSE, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, ForLoop) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::FOR, "for", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "i", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "0", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "i", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS, "<", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "10", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "i", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "i", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_BRACE, "{", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_BRACE, "}", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(
        cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::GET_LOCAL, 0,
        cppLox::ByteCode::Opcode::CONSTANT, 1, cppLox::ByteCode::Opcode::LESS, cppLox::ByteCode::Opcode::JUMP_IF_FALSE,
        0, 18, cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::JUMP, 0, 11,
        cppLox::ByteCode::Opcode::GET_LOCAL, 0, cppLox::ByteCode::Opcode::CONSTANT, 2, cppLox::ByteCode::Opcode::ADD,
        cppLox::ByteCode::Opcode::SET_LOCAL, 0, cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::LOOP, 0, 23,
        cppLox::ByteCode::Opcode::LOOP, 0, 14, cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::POP,
        cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, GreaterThanExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER, ">", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::GREATER, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, GreaterThanEqualExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::GREATER_EQUAL, ">=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::GREATER_EQUAL, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, GroupingExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PLUS, "+", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "3", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::ADD, cppLox::ByteCode::Opcode::CONSTANT, 2,
                                       cppLox::ByteCode::Opcode::MULTIPLY, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, IfStatement) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IF, "if", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::JUMP_IF_FALSE, 0, 1,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, IfElseStatement) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IF, "if", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::PRINT, "print", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::ELSE, "else", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(
        cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::JUMP_IF_FALSE, 0, 8, cppLox::ByteCode::Opcode::POP,
        cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::PRINT, cppLox::ByteCode::Opcode::POP,
        cppLox::ByteCode::Opcode::JUMP, 0, 0, cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, LessThanExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS, "<", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::LESS, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, LessThanEqualExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LESS_EQUAL, "<=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::LESS_EQUAL, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, MultiplicationExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::STAR, "*", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::MULTIPLY, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, NegateExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::NEGATE,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::NULL_,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, NotExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG, "!", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::NOT,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::NULL_,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, NotEqualExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::BANG_EQUAL, "!=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::NOT_EQUAL, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, NullLiteralExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NULL_, "null", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, SubtractionExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::MINUS, "-", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "2", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::CONSTANT, 1,
                                       cppLox::ByteCode::Opcode::SUBTRACT, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, TrueLiteralExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, VariableDeclarationExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::DEFINE_GLOBAL, 0,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, VariableAssignmentExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 1, cppLox::ByteCode::Opcode::SET_GLOBAL, 0,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::NULL_,
                                       cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, LocalVariableAssignmentExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_BRACE, "{", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_BRACE, "}", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 0, cppLox::ByteCode::Opcode::POP,
                                       cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, VariableDeclarationAndAssignmentExpression) {
    // Arrange
    tokens = {
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::VAR, "var", 1),
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::IDENTIFIER, "a", 1),
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::EQUAL, "=", 1),
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::NUMBER, "1", 1),
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::SEMICOLON, ";", 1),
        cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1),
    };

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::CONSTANT, 1, cppLox::ByteCode::Opcode::DEFINE_GLOBAL,
                                       0, cppLox::ByteCode::Opcode::NULL_, cppLox::ByteCode::Opcode::RETURN);
}

TEST_F(CompilerIntegrationTest, WhileLoopExpression) {
    // Arrange
    tokens = {cppLox::Frontend::Token(cppLox::Frontend::Token::Type::WHILE, "while", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_PARENTHESES, "(", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::TRUE, "true", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_PARENTHESES, ")", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::LEFT_BRACE, "{", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::RIGHT_BRACE, "}", 1),
              cppLox::Frontend::Token(cppLox::Frontend::Token::Type::END_OF_FILE, "", 1)};

    // Act
    objectFunction = compiler->compile(tokens);

    // Assert
    assertChunkContaintsExactlyInOrder(cppLox::ByteCode::Opcode::TRUE, cppLox::ByteCode::Opcode::JUMP_IF_FALSE, 0, 4,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::LOOP, 0, 8,
                                       cppLox::ByteCode::Opcode::POP, cppLox::ByteCode::Opcode::NULL_,
                                       cppLox::ByteCode::Opcode::RETURN);
}
