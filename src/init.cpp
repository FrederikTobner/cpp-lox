#include "init.hpp"

#include <fstream>
#include <iostream>
#include <memory>

#include "bytecode/chunk.hpp"
#include "error/compiletime_exception.hpp"
#include "error/runtime_exception.hpp"
#include "exit_code.hpp"

auto cppLox::repl() -> void {
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
            cppLox::run(line, lexer, memoryMutator.get(), compiler, vm);
        } catch (cppLox::Error::CompileTimeException e) {
            std::cout << e.what() << std::endl;
        } catch (cppLox::Error::RunTimeException e) {
            std::cout << e.what() << std::endl;
        }
    }
}

auto cppLox::runFile(char const * path) -> void {
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
        cppLox::run(source, lexer, memoryMutator.get(), compiler, vm);
    } catch (cppLox::Error::CompileTimeException e) {
        std::cout << e.what() << std::endl;
        exit(cppLox::EXIT_CODE_COMPILATION_ERROR);
    } catch (cppLox::Error::RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(cppLox::EXIT_CODE_RUNTIME_ERROR);
    }
}

auto cppLox::run(std::string & source, cppLox::Frontend::Lexer & lexer, MemoryMutator * memoryMutator,
                 cppLox::Frontend::Compiler & compiler, cppLox::Backend::VM & vm) -> void {
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    // Source file is not needed after the tokens are generated
    source.clear();
    std::unique_ptr<cppLox::ByteCode::Chunk> chunk = compiler.compile(tokens);
    // Tokens are not needed after the chunk is generated
    tokens.clear();
    vm.interpret(*chunk.get());
}