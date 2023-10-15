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
    VM(MemoryMutator * memoryMutator);
    /// @brief Destructor of the virtual machine
    ~VM() = default;
    /// @brief Interprets the given chunk
    /// @param chunk The chunk to interpret
    auto interpret(cppLox::ByteCode::Chunk & chunk) -> void;
    /// @brief Pushes the given value onto the stack
    /// @param value The value to push onto the stack
    auto push(cppLox::Types::Value value) -> void;
    /// @brief Pops the top value from the stack
    [[nodiscard]] auto pop() -> cppLox::Types::Value;

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
    MemoryMutator * m_memoryMutator;
};
} // namespace cppLox::Backend
