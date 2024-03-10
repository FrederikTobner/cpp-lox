#pragma once

namespace cppLox::Frontend {
/// @brief The type of a function.
enum class FunctionType {
    /// @brief The type of a function
    FUNCTION,
    /// @brief The type of top-level code
    SCRIPT,
};

} // namespace cppLox::Frontend
