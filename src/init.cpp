#include "init.hpp"

#include "chunk.hpp"
#include "compiler.hpp"
#include "compiletime_exception.hpp"
#include "exit_code.hpp"
#include "lexer.hpp"
#include "runtime_exception.hpp"
#include "token.hpp"
#include "vm.hpp"

#include <fstream>
#include <iostream>
#include <memory>

static auto run(std::string & source, Lexer & lexer, MemoryMutator * memoryMutator, VM & vm, Compiler & compiler)
    -> void;

auto repl() -> void {
    Lexer lexer;
    MemoryMutator * memoryMutator = new MemoryMutator();
    VM vm(memoryMutator);
    Compiler compiler(memoryMutator);
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
    Lexer lexer;
    MemoryMutator * memoryMutator = new MemoryMutator();
    VM vm(memoryMutator);
    Compiler compiler(memoryMutator);
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

static auto run(std::string & source, Lexer & lexer, MemoryMutator * memoryMutator, VM & vm, Compiler & compiler)
    -> void {
    try {
        std::vector<Token> tokens = lexer.tokenize(source);
        // Source file is not needed after the tokens are generated
        source.clear();
        std::unique_ptr<Chunk> chunk = compiler.compile(tokens);
        // Tokens are not needed after the chunk is generated
        tokens.clear();
        vm.interpret(*chunk.get());
    } catch (CompileTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_COMPILATION_ERROR);
    } catch (RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_RUNTIME_ERROR);
    }
}