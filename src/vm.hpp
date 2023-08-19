#pragma once

#include <format>

#include "chunk.hpp"
#include "value.hpp"

/// @brief The maximum amount of values that can be stored on the stack
#define STACK_MAX 256

/// @brief The virtual machine used by the cpplox interpreter
class VM {

  public:
    /// @brief Constructs a new virtual machine
    VM();
    /// @brief Destructor of the virtual machine
    ~VM();
    /// @brief Interprets the given chunk
    /// @param chunk The chunk to interpret
    void interpret(Chunk & chunk);
    /// @brief Pushes the given value onto the stack
    /// @param value The value to push onto the stack
    void push(Value value);
    /// @brief Pops the top value from the stack
    [[nodiscard]] Value pop();

  private:
    /// @brief Throws a runtime exception with the given message
    /// @tparam ...Args The types of the arguments
    /// @param fmt The format string
    /// @param ...args The arguments to the format string
    template <class... Args> void runTimeError(std::string_view fmt, Args &&... args);

    /// @brief The stack
    Value m_stack[STACK_MAX];

    /// @brief The index of the top of the stack
    size_t m_stack_top;

    /// @brief The index of the currently executed instruction
    size_t m_instruction_index;

    /// @brief The chunk to execute
    Chunk * m_chunk;
};
