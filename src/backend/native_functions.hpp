#pragma once

#include <chrono>
#include <functional>

#include "../types/value.hpp"
#include "callframe.hpp"

namespace cppLox::Backend {

cppLox::Types::Value clock(uint8_t argCount, Types::Value * args, CallFrame & frame,
                           std::function<void(CallFrame & frame, std::string_view fmt)> onError);
} // namespace cppLox::Backend