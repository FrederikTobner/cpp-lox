#pragma once

#include <stack>

#include "chunk.hpp"

#define STACK_MAX 256

class VM {

  private:
    size_t m_instruction_index;
    Value m_stack[STACK_MAX];
    size_t m_stack_top;

  public:
    typedef enum {
        INTERPRET_OK,
        INTERPRET_COMPILE_ERROR,
        INTERPRET_RUNTIME_ERROR
    } InterpretResult;
    VM();
    ~VM();
    [[nodiscard]] InterpretResult interpret(Chunk & chunk);
    void push(Value value);
    [[nodiscard]] Value pop();
};
