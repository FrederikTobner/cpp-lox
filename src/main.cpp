#include <format>
#include <iostream>

#include "exit_code.hpp"
#include "init.hpp"

#include "cpplox-config.hpp"

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
