#include "init.hpp"

#include "chunk.hpp"
#include "compiler.hpp"
#include "compiletime_exception.hpp"
#include "exit_code.hpp"
#include "lexer.hpp"
#include "opcode.hpp"
#include "runtime_exception.hpp"
#include "token.hpp"
#include "value.hpp"
#include "vm.hpp"

#include <fstream>
#include <iostream>
#include <memory>

static void run(const std::string & source);

void repl() {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit" || line == "") {
            break;
        }
        run(line);
    }
}

void runFile(const char * path) {
    std::string source;
    std::ifstream file;
    file.open(path);
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        source.append(line);
    }
    run(source);
}

static void run(const std::string & source) {
    VM vm;
    Lexer lexer;
    Compiler compiler;
    try {
        std::vector<Token> tokens = lexer.tokenize(source);
        std::unique_ptr<Chunk> chunk = compiler.compile(tokens);
        vm.interpret(*chunk.get());
    } catch (CompileTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_COMPILATION_ERROR);
    } catch (RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_CODE_RUNTIME_ERROR);
    }
}