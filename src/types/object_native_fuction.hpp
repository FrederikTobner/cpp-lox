#pragma once

#include <functional>

#include "../backend/callframe.hpp"
#include "object.hpp"
#include "value.hpp"

namespace cppLox::Types {

class ObjectNativeFunction : public Object {
  public:
    ObjectNativeFunction(
        std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                            std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)>
            function);
    ~ObjectNativeFunction() = default;

    /// @brief Writes the native function to the output stream.
    /// @param os The output stream to write to.
    /// @return The output stream.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << "<native fn>";
    }

    auto call(uint8_t argCount, Value * args, cppLox::Backend::CallFrame & frame,
              std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError) -> Value {
        return m_function(argCount, args, frame, onError);
    }

  private:
    std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                        std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)>
        m_function;
};

} // namespace cppLox::Types