#pragma once

namespace cppLox {

/// @brief Exit codes based on the sysexits from UNIX
typedef enum {
    /// Signals a successful exit
    EXIT_CODE_SUCCESS = 0,
    /// Signals a command line error usage
    EXIT_CODE_COMMAND_LINE_USAGE_ERROR = 64,
    /// Signals an error that occured during compile time
    EXIT_CODE_COMPILATION_ERROR = 65,
    /// Signals an error that occured during runtime time
    EXIT_CODE_RUNTIME_ERROR = 70,
    /// Signals an error that occured during system time
    EXIT_CODE_SYSTEM_ERROR = 71,
    /// Signals an error regarding the input or the output
    EXIT_CODE_INPUT_OUTPUT_ERROR = 74
} exit_code_t;

} // namespace cppLox
