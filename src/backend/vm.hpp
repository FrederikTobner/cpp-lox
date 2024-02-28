#pragma once

#include <format>

#include "../bytecode/chunk.hpp"
#include "../memory_mutator.hpp"
#include "../types/value.hpp"

namespace cppLox::Backend {

/// @brief The maximum amount of values that can be stored on the stack
#define STACK_MAX 256

/// @brief The virtual machine used by the cpplox interpreter
class VM {

  public:
    /// @brief Constructs a new virtual machine
    /// @param memoryMutator The memory mutator that is used by the virtual machine
    VM(std::shared_ptr<cppLox::MemoryMutator> memoryMutator);

    /// @brief Destructor of the virtual machine
    ~VM() = default;

    /// @brief Gets two bytes from the chunk and returns them as a 16-bit integer
    /// @param chunk The chunk to get the bytes from
    /// @param offset The offset to get the bytes from
    /// @return The 16-bit integer
    [[nodiscard]] auto getShort(cppLox::ByteCode::Chunk & chunk, size_t offset) -> uint16_t;

    /// @brief Interprets the given chunk
    /// @param chunk The chunk to interpret
    auto interpret(cppLox::ByteCode::Chunk & chunk) -> void;

    /// @brief Peeks at the top value of the stack
    /// @return The top value of the stack
    [[nodiscard]] auto peek() -> cppLox::Types::Value;

    /// @brief Pops the top value from the stack
    /// @return The top value of the stack
    [[nodiscard]] auto pop() -> cppLox::Types::Value;

    /// @brief Pushes the given value onto the stack
    /// @param value The value to push onto the stack
    auto push(cppLox::Types::Value value) -> void;

    /// @brief Resets the stack
    auto resetStack() -> void;

  private:
    /// @brief Throws a runtime exception with the given message
    /// @tparam ...Args The types of the arguments
    /// @param fmt The format string
    /// @param ...args The arguments to the format string
    template <class... Args> auto runTimeError(std::string_view fmt, Args &&... args) -> void;

    /// @brief The stack
    cppLox::Types::Value m_stack[STACK_MAX];

    /// @brief The index of the top of the stack
    size_t m_stack_top;

    /// @brief The index of the currently executed instruction
    size_t m_instruction_index;

    /// @brief The chunk to execute
    cppLox::ByteCode::Chunk * m_chunk;

    /// @brief The memory manager.
    std::shared_ptr<cppLox::MemoryMutator> m_memoryMutator;
};
} // namespace cppLox::Backend
