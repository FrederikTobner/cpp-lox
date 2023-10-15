#pragma once

#include <memory>
#include <string>
#include <string_view>


#include "types/object_string.hpp"

namespace cppLox::Common {

/// @brief Hashing for strings, string_views and char pointers.
struct StringHash {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    /// @brief Hashes the given sequence of characters.
    /// @param str The sequence of characters to hash.
    /// @return The hash of the given sequence of characters.
    [[nodiscard]] auto operator()(char const * str) const -> std::size_t;

    /// @brief Hashes the given string view.
    /// @param str The string view to hash.
    /// @return The hash of the given string view.
    [[nodiscard]] auto operator()(std::string_view str) const -> std::size_t;

    /// @brief Hashes the given string.
    /// @param str The string to hash.
    /// @return The hash of the given string.
    [[nodiscard]] auto operator()(std::string const & str) const -> std::size_t;

    /// @brief Hashes the given object string.
    /// @param str The object string to hash.
    /// @return The hash of the given object string.
    [[nodiscard]] auto operator()(cppLox::Types::ObjectString const & str) const -> std::size_t;

    [[nodiscard]] auto operator()(std::unique_ptr<cppLox::Types::ObjectString> str) const -> std::size_t;
};
} // namespace cppLox::Common