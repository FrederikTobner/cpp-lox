#include "native_functions.hpp"

using namespace cppLox::Backend;

cppLox::Types::Value cppLox::Backend::clock(uint8_t argCount, cppLox::Types::Value * args, CallFrame & frame,
                                            std::function<void(CallFrame & frame, std::string_view fmt)> onError) {
    return cppLox::Types::Value(static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count()));
}