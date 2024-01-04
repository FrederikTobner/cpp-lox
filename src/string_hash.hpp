#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "types/object_string.hpp"

namespace std {

template <> struct hash<cppLox::Types::ObjectString> {

    /// @brief Hashes the given ObjectString reference.
    /// @param s The ObjectString to hash.
    /// @return The hash of the given string.
    [[nodiscard]] auto operator()(cppLox::Types::ObjectString & s) const -> std::size_t {
        return std::hash<std::string_view>{}(s.string());
    }

    /// @brief Hashes the given ObjectString pointer.
    /// @param s The ObjectString pointer to hash.
    /// @return The hash of the given string.
    [[nodiscard]] auto operator()(cppLox::Types::ObjectString * s) const -> std::size_t {
        return std::hash<std::string_view>{}(s->string());
    }
};
} // namespace std