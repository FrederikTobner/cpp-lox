#pragma once

#include <format>

#include "object.hpp"
#include "object_string.hpp"

/// @brief Formatter for the Object class.
template <> struct std::formatter<cppLox::Types::Object *> : std::formatter<std::string> {
    /// @brief Formats the object as a string.
    /// @param object The object to format.
    /// @param ctx The format context.
    /// @return The formatted string.
    [[nodiscard]] auto format(cppLox::Types::Object * object, format_context & ctx) const {
        switch (object->type()) {
        case cppLox::Types::Object::Type::STRING:
            return formatter<string>::format(std::format("{}", object->as<cppLox::Types::ObjectString>()->string()),
                                             ctx);
        }
        // should be be unreachable
        return formatter<string>::format(std::format("undefined"), ctx);
    }
};