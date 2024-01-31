#include "local.hpp"

cppLox::Frontend::Local::Local(cppLox::Frontend::Token & token, std::int32_t depth) : m_depth(depth), m_token(token) {
}

cppLox::Frontend::Token cppLox::Frontend::Local::getToken() const {
    return m_token;
}

int32_t cppLox::Frontend::Local::getDepth() const {
    return m_depth;
}