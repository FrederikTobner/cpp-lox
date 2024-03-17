#include "object_native_fuction.hpp"

using namespace cppLox::Types;

ObjectNativeFunction::ObjectNativeFunction(
    std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                        std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)>
        function)
    : m_function(function) {
    m_type = Object::Type::NATIVE_FUNCTION;
}