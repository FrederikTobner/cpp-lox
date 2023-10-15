#include "init.hpp"

#include "backend/vm.hpp"
#include "bytecode/chunk.hpp"
#include "error/compiletime_exception.hpp"
#include "error/runtime_exception.hpp"
#include "exit_code.hpp"
#include "frontend/compiler.hpp"
#include "frontend/lexer.hpp"
#include "frontend/token.hpp"

#include <fstream>
#include <iostream>
#include <memory>

/// @brief The main entry point of the program.
/// @param source The source code to run.
/// @param lexer The lexer that will tokenize the source code.
/// @param memoryMutator The memory mutator that will be used by the VM and the compiler to allocate memory.
/// @param compiler The compiler that will compile the source code into bytecode.
/// @param vm The VM that will interpret the bytecode.
/// @return The exit code of the program.
static auto run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
                cppLox::Frontend::Compiler & compiler, cppLox::Backend::VM & vm) -> void;

auto repl() -> void {
    cppLox::Frontend::Lexer lexer;
    std::unique_ptr<MemoryMutator> memoryMutator = std::make_unique<MemoryMutator>();
    cppLox::Backend::VM vm(memoryMutator.get());
    cppLox::Frontend::Compiler compiler(memoryMutator.get());
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit" || line == "") {
            break;
        }
        try {
            run(line, lexer, memoryMutator.get(), compiler, vm);
        } catch (cppLox::Error::CompileTimeException e) {
            std::cout << e.what() << std::endl;
        } catch (cppLox::Error::RunTimeException e) {
            std::cout << e.what() << std::endl;
        }
    }
}

auto runFile(char const * path) -> void {
    cppLox::Frontend::Lexer lexer;
    std::unique_ptr<MemoryMutator> memoryMutator = std::make_unique<MemoryMutator>();
    cppLox::Backend::VM vm(memoryMutator.get());
    cppLox::Frontend::Compiler compiler(memoryMutator.get());
    std::string source;
    std::ifstream file;
    file.open(path);
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        source.append(line);
    }
    try {
        run(source, lexer, memoryMutator.get(), compiler, vm);
    } catch (cppLox::Error::CompileTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_COMPILATION_ERROR);
    } catch (cppLox::Error::RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_RUNTIME_ERROR);
    }
}

static auto run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
                cppLox::Frontend::Compiler & compiler, cppLox::Backend::VM & vm) -> void {
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    // Source file is not needed after the tokens are generated
    source.clear();
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler.compile(tokens);
    // Tokens are not needed after the chunk is generated
    tokens.clear();
    vm.interpret(*chunk.get());
}