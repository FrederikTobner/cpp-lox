#include <gtest/gtest.h>

#include "../../src/string_hash.hpp"

TEST(StringHash, HashesObjectString) {
    std::hash<cppLox::Types::ObjectString> hash;
    cppLox::Types::ObjectString str("Hello, world!");
    cppLox::Types::ObjectString str2("Hello, world!");
    ASSERT_EQ(hash(&str), hash(&str2));
}