#include <gtest/gtest.h>

#include "../../src/string_hash.hpp"

TEST(StringHash, HashesCharPointer) {
    cppLox::Common::StringHash hash;
    char const * str = "Hello, world!";
    char const * str2 = "Hello, world!";
    ASSERT_EQ(hash(str), hash(str2));
}

TEST(StringHash, HashesStringView) {
    cppLox::Common::StringHash hash;
    std::string_view str = "Hello, world!";
    std::string_view str2 = "Hello, world!";
    ASSERT_EQ(hash(str), hash(str2));
}

TEST(StringHash, HashesString) {
    cppLox::Common::StringHash hash;
    std::string str = "Hello, world!";
    std::string str2 = "Hello, world!";
    ASSERT_EQ(hash(str), hash(str2));
}