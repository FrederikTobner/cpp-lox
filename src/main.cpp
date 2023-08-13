#include <iostream>

#include "exit_code.hpp"
#include "init.hpp"

#include "cpplox-config.hpp"

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cout << "Usage: " << PROJECT_NAME << " [script]" << std::endl;
        exit(EXIT_CODE_COMMAND_LINE_USAGE_ERROR);
    }
    return 0;
}
