#include "local.hpp"

cppLox::Frontend::Local::Local(cppLox::Frontend::Token & token, std::int32_t depth) : m_depth(depth), m_token(token) {
}

auto cppLox::Frontend::Local::getToken() const noexcept -> cppLox::Frontend::Token const & {
    return m_token;
}

auto cppLox::Frontend::Local::getDepth() const noexcept -> int32_t {
    return m_depth;
}