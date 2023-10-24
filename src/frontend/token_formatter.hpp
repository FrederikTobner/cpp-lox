#pragma once

#include <format>
#include <string>

#include "token.hpp"

/// @brief Formatter for the Token class
template <> struct std::formatter<cppLox::Frontend::Token> : std::formatter<std::string_view> {
    [[nodiscard]] auto format(cppLox::Frontend::Token token, format_context & ctx) const {
        return formatter<string_view>::format(std::format("Token({}, {})", token.lexeme(), token.line()), ctx);
    }
};