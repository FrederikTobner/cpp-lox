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

static auto run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
                cppLox::Backend::VM & vm, cppLox::Frontend::Compiler & compiler) -> void;

auto repl() -> void {
    cppLox::Frontend::Lexer lexer;
    MemoryMutator * memoryMutator = new MemoryMutator();
    cppLox::Backend::VM vm(memoryMutator);
    cppLox::Frontend::Compiler compiler(memoryMutator);
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit" || line == "") {
            break;
        }
        run(line, lexer, memoryMutator, vm, compiler);
    }
    delete memoryMutator;
}

auto runFile(char const * path) -> void {
    cppLox::Frontend::Lexer lexer;
    MemoryMutator * memoryMutator = new MemoryMutator();
    cppLox::Backend::VM vm(memoryMutator);
    cppLox::Frontend::Compiler compiler(memoryMutator);
    std::string source;
    std::ifstream file;
    file.open(path);
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        source.append(line);
    }
    run(source, lexer, memoryMutator, vm, compiler);
    delete memoryMutator;
}

static auto run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
                cppLox::Backend::VM & vm, cppLox::Frontend::Compiler & compiler) -> void {
    try {
        std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
        // Source file is not needed after the tokens are generated
        source.clear();
        std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler.compile(tokens);
        // Tokens are not needed after the chunk is generated
        tokens.clear();
        vm.interpret(*chunk.get());
    } catch (cppLox::Error::CompileTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_COMPILATION_ERROR);
    } catch (cppLox::Error::RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_RUNTIME_ERROR);
    }
}