/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file init.cpp
 * @brief This file contains the implementation of the init functions.
 */

#include "init.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "bytecode/chunk.hpp"
#include "error/runtime_exception.hpp"
#include "exit_code.hpp"

auto cppLox::repl() -> void {
    cppLox::Frontend::Lexer lexer;
    std::shared_ptr<MemoryMutator> memoryMutator = std::make_shared<MemoryMutator>();
    cppLox::Backend::VM vm(memoryMutator);
    cppLox::Frontend::Compiler compiler(memoryMutator);
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit" || line == "") {
            break;
        }
        try {
            cppLox::run(line, lexer, compiler, vm);
        } catch (cppLox::Error::RunTimeException e) {
            std::cout << e.what() << std::endl;
        }
    }
}

auto cppLox::runFile(char const * path) -> void {
    cppLox::Frontend::Lexer lexer;
    std::shared_ptr<MemoryMutator> memoryMutator = std::make_shared<MemoryMutator>();
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
    try {
        cppLox::run(source, lexer, compiler, vm);
    } catch (cppLox::Error::RunTimeException e) {
        std::cout << e.what() << std::endl;
        exit(cppLox::EXIT_CODE_RUNTIME_ERROR);
    }
}

auto cppLox::run(std::string & source, cppLox::Frontend::Lexer & lexer, cppLox::Frontend::Compiler & compiler,
                 cppLox::Backend::VM & vm) -> void {
    std::vector<cppLox::Frontend::Token> tokens = lexer.tokenize(source);
    source.clear();
    std::optional<cppLox::Types::ObjectFunction *> main_fun = compiler.compile(tokens);
    tokens.clear();
    if (!main_fun.has_value()) {
        return;
    }
    vm.interpret(*(main_fun.value()));
}