#include <gtest/gtest.h>

#include "../../src/string_hash.hpp"

TEST(StringHash, HashesObjectString) {
    // Arrange
    std::hash<cppLox::Types::ObjectString> hash;
    cppLox::Types::ObjectString str("Hello, world!");
    cppLox::Types::ObjectString str2("Hello, world!");

    // Act
    auto firstHash = hash(str);
    auto secondHash = hash(str2);

    // Assert
    ASSERT_EQ(firstHash, secondHash);
}
