#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "types/object_string.hpp"

namespace std {

template <> struct hash<cppLox::Types::ObjectString> {

    [[nodiscard]] auto operator()(cppLox::Types::ObjectString & s) const -> std::size_t {
        return std::hash<std::string_view>{}(s.string());
    }

    [[nodiscard]] auto operator()(cppLox::Types::ObjectString * s) const -> std::size_t {
        return std::hash<std::string_view>{}(s->string());
    }
};
} // namespace std