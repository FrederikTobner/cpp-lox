#pragma once

#include <string>

#include <cstddef>

#include "token.hpp"

namespace cppLox::Frontend {

// Forward declaration to avoid circular dependency
class CompilationScope;

/// @brief Models a local variable.
class Local {

    friend class CompilationScope;

  public:
    /// @brief Constructor of the local variable.
    /// @param token The token of the name of the local variable.
    /// @param depth The depth of the local variable.
    Local(cppLox::Frontend::Token & token, std::int32_t depth);

    /// @brief Gets the token of the name of the local variable.
    cppLox::Frontend::Token getToken() const;

    /// @brief Gets the depth of the local variable.
    std::int32_t getDepth() const;

  private:
    /// @brief The token of the name of the local variable.
    cppLox::Frontend::Token m_token;

    /// @brief The depth of the local variable.
    std::int32_t m_depth;
};
} // namespace cppLox::Frontend