#include "string_hash.hpp"

using namespace cppLox::Common;

[[nodiscard]] auto StringHash::operator()(char const * str) const -> std::size_t {
    return hash_type{}(str);
}

[[nodiscard]] auto StringHash::operator()(std::string_view str) const -> std::size_t {
    return hash_type{}(str);
}

[[nodiscard]] auto StringHash::operator()(std::string const & str) const -> std::size_t {
    return hash_type{}(str);
}

[[nodiscard]] auto StringHash::operator()(cppLox::Types::ObjectString const & str) const -> std::size_t {
    return hash_type{}(str.string());
}

[[nodiscard]] auto StringHash::operator()(std::unique_ptr<cppLox::Types::ObjectString> str) const -> std::size_t {
    return hash_type{}(str.get()->string());
}