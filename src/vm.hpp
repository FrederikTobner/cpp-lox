#pragma once

#include <stack>

#include "chunk.hpp"

#define STACK_MAX 256

class VM {

  public:
    /// @brief The result of interpreting a chunk of bytecode
    enum InterpretResult {
        INTERPRET_OK,
        INTERPRET_COMPILE_ERROR,
        INTERPRET_RUNTIME_ERROR
    };
    VM();
    ~VM();
    [[nodiscard]] InterpretResult interpret(Chunk & chunk);
    void push(Value value);
    [[nodiscard]] Value pop();

  private:
    Value m_stack[STACK_MAX];
    size_t m_stack_top;
};
