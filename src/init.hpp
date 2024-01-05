#pragma once

#include <string>

#include "backend/vm.hpp"
#include "frontend/compiler.hpp"
#include "frontend/lexer.hpp"
#include "frontend/token.hpp"

namespace cppLox {

/// @brief Starts the REPL
auto repl() -> void;

/// @brief Executes the program declared in the given file
/// @param path The path to the file
auto runFile(char const * path) -> void;

/// @brief The main entry point of the program.
/// @param source The source code to run.
/// @param lexer The lexer that will tokenize the source code.
/// @param memoryMutator The memory mutator that will be used by the VM and the compiler to allocate memory.
/// @param compiler The compiler that will compile the source code into bytecode.
/// @param vm The VM that will interpret the bytecode.
/// @return The exit code of the program.
auto run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
         cppLox::Frontend::Compiler & compiler, cppLox::Backend::VM & vm) -> void;

} // namespace cppLox