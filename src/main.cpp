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
 * @file main.cpp
 * @brief This file contains the main entry point of the program.
 */

#include <format>
#include <iostream>

#include "cpplox-config.hpp"
#include "exit_code.hpp"
#include "init.hpp"

/// @brief The main entry point of the program.
/// @param argc The number of arguments passed to the program.
/// @param argv The arguments passed to the program.
/// @return The exit code of the program.
auto main(int argc, char const ** argv) -> int {
    if (argc == 1) {
        cppLox::repl();
    } else if (argc == 2) {
        cppLox::runFile(argv[1]);
    } else {
        std::cout << std::format("Usage: {} [script]", PROJECT_NAME) << std::endl;
        exit(cppLox::EXIT_CODE_COMMAND_LINE_USAGE_ERROR);
    }
    return 0;
}
