#pragma once

#include <format>
#include <string>

#include "value.hpp"

/// @brief Formatter for the Value class
template <> struct std::formatter<cppLox::Types::Value> : std::formatter<std::string_view> {
    [[nodiscard]] auto format(cppLox::Types::Value value, format_context & ctx) const {
        switch (value.getType()) {
        case cppLox::Types::Value::Type::BOOL:
            return formatter<string_view>::format(std::format("{}", value.as<bool>()), ctx);
        case cppLox::Types::Value::Type::NULL_:
            return formatter<string_view>::format(std::format("null"), ctx);
        case cppLox::Types::Value::Type::NUMBER:
            return formatter<string_view>::format(std::format("{}", value.as<double>()), ctx);
        case cppLox::Types::Value::Type::OBJECT:
            return formatter<string_view>::format(std::format("{}", value.as<cppLox::Types::Object *>()), ctx);
        }
        // should be be unreachable
        return formatter<string_view>::format(std::format("undefined"), ctx);
    }
};