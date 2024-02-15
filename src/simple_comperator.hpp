#pragma once

#include "types/object_string.hpp"

namespace cppLox::Types {

template <typename T>
concept HasEqualOperator = requires(T & a, T & b) { a == b; };

/// @brief Simple comperator for comparing two references or two pointers.
/// @tparam T The type of the references / pointers.
template <HasEqualOperator T> struct SimpleComperator {
    /// @brief Compares two references.
    /// @param a The first reference.
    /// @param b The second reference.
    /// @return True if the references are equal, false otherwise.
    [[nodiscard]] auto operator()(T const & a, T const & b) const -> bool {
        return a == b;
    }

    /// @brief Compares two pointers.
    /// @param a The first pointer.
    /// @param b The second pointer.
    /// @return True if the pointers are equal, false otherwise.
    [[nodiscard]] auto operator()(T const * a, T const * b) const -> bool {
        return (*a) == b;
    }
};
} // namespace cppLox::Types
