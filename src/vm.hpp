#pragma once

#include <format>

#include "chunk.hpp"
#include "value.hpp"

#define STACK_MAX 256

class VM {

  public:
    VM();
    ~VM();
    void interpret(Chunk & chunk);
    void push(Value value);
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
