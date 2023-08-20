#pragma once

#include <format>
#include <string>

#include "token.hpp"

/// @brief Formatter for the Token class
template <> struct std::formatter<Token> : std::formatter<std::string> {
    auto format(Token token, format_context & ctx) const {
        return formatter<string>::format(std::format("Token({}, {})", token.lexeme(), token.line()), ctx);
    }
};