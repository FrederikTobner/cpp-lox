#pragma once

/// @brief Starts the REPL
auto repl() -> void;

/// @brief Executes the program declared in the given file
/// @param path The path to the file
auto runFile(char const * path) -> void;