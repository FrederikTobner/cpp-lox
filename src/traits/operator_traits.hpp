#pragma once

namespace cppLox::Traits {

template <typename T>
concept HasEqualOperator = requires(T & a, T & b) { a == b; };
} // namespace cppLox::Traits
