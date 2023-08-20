#pragma once

#include <format>
#include <string>

#include "opcode.hpp"

/// @brief Formatter for the Opcode enum
template <> struct std::formatter<Opcode> : std::formatter<std::string> {
    /// @brief Formats the opcode as a string.
    /// @param opcode The opcode to format.
    /// @param ctx The format context.
    /// @return The formatted string.
    [[nodiscard]] inline auto format(Opcode opcode, format_context & ctx) const {
        return formatter<string>::format(std::format("{}", opcode_as_string(opcode)), ctx);
    }
};