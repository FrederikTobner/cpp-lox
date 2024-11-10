#include <gtest/gtest.h>

#include "../../src/frontend/upvalue.hpp"

class UpValueTest : public ::testing::Test {
  protected:
    UpValueTest() : upValue(1, true) {
    }

    cppLox::Frontend::Upvalue upValue;
};

TEST_F(UpValueTest, Index) {
    // Act
    auto index = upValue.index();

    // Assert
    ASSERT_EQ(index, 1);
}

TEST_F(UpValueTest, IsLocal) {
    // Act
    auto isLocal = upValue.isLocal();

    // Assert
    ASSERT_TRUE(isLocal);
}