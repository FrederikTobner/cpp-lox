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
 * @file init.hpp
 * @brief This file contains the declarations for the inital functions of the interpreter.
 */

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
auto run(std::string & source, cppLox::Frontend::Lexer & lexer, cppLox::Frontend::Compiler & compiler,
         cppLox::Backend::VM & vm) -> void;

} // namespace cppLox