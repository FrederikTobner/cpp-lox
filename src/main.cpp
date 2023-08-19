#include <format>
#include <iostream>

#include "exit_code.hpp"
#include "init.hpp"

#include "cpplox-config.hpp"

auto main(int argc, char const * argv[]) -> int {
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cout << std::format("Usage: {} [script]", PROJECT_NAME) << std::endl;
        exit(EXIT_CODE_COMMAND_LINE_USAGE_ERROR);
    }
    return 0;
}
