#pragma once

#include "types/object_string.hpp"

namespace cppLox::Types {
template <typename T> struct SimpleComperator {
    [[nodiscard]] auto operator()(T const & a, T const & b) const -> bool {
        return a == b;
    }

    [[nodiscard]] auto operator()(T const * a, T const * b) const -> bool {
        return a->operator==(b);
    }
};
} // namespace cppLox::Types
