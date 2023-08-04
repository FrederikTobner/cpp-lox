#include "chunk.hpp"
#include "opcode.hpp"
#include "value.hpp"
#include "vm.hpp"

int main(int argc, char const * argv[]) {
    Chunk chunk;
    chunk.write(OP_CONSTANT, 1);
    chunk.write(chunk.addConstant(1.2), 1);
    chunk.write(OP_RETURN, 2);
    chunk.disassemble("test chunk");
    return 0;
}
