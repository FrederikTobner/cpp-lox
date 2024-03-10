#pragma once

#include <format>

#include "object.hpp"
#include "object_function.hpp"
#include "object_string.hpp"

/// @brief Formatter for the Object class.
template <> struct std::formatter<cppLox::Types::Object *> : std::formatter<std::string_view> {

    /// @brief Formats the object as a string.
    /// @param object The object to format.
    /// @param ctx The format context.
    /// @return The formatted string.
    [[nodiscard]] auto format(cppLox::Types::Object * object, format_context & ctx) const {
        switch (object->type()) {
        case cppLox::Types::Object::Type::STRING:
            return formatter<string_view>::format(
                std::format("{}", object->as<cppLox::Types::ObjectString>()->string()), ctx);
        case cppLox::Types::Object::Type::FUNCTION:
            {
                cppLox::Types::ObjectString * name = object->as<cppLox::Types::ObjectFunction>()->name();
                if (name != nullptr) {
                    return formatter<string_view>::format(std::format("<fn {}>", name->string()), ctx);
                }
                return formatter<string_view>::format(std::format("<script>"), ctx);
            }
        }
        // should be be unreachable
        return formatter<string_view>::format(std::format("undefined"), ctx);
    }
};