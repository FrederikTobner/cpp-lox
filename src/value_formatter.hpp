#pragma once

#include <format>
#include <string>

#include "value.hpp"

/// @brief Formatter for the Value class
template <> struct std::formatter<Value> : std::formatter<std::string> {
    auto format(Value value, format_context & ctx) const {
        switch (value.getType()) {
        case Value::Type::BOOL:
            return formatter<string>::format(std::format("{}", value.as<bool>()), ctx);
        case Value::Type::NULL_:
            return formatter<string>::format(std::format("null"), ctx);
        case Value::Type::NUMBER:
            return formatter<string>::format(std::format("{}", value.as<double>()), ctx);
        case Value::Type::OBJECT:
            return formatter<string>::format(std::format("{}", value.as<Object *>()), ctx);
        }
        // should be be unreachable
        return formatter<string>::format(std::format("undefined"), ctx);
    }
};