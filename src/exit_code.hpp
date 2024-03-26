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
 * @file exit_code.hpp
 * @brief This file contains the exit codes for the program.
 */

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
