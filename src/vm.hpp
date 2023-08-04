#pragma once

#include "chunk.hpp"

class VM {
  private:
    /* data */
  public:
    VM();
    ~VM();
    void execute(const Chunk & chunk);
};
