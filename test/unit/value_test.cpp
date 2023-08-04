#include "../../src/value.hpp"
#include "gtest/gtest.h"

TEST(ValueTest, DefaultConstructor) {
    Value v;
    EXPECT_EQ(v.getType(), Value::VAL_NIL);
}

TEST(ValueTest, BoolConstructor) {
    Value v(true);
    EXPECT_EQ(v.getType(), Value::VAL_BOOL);
    EXPECT_EQ(v.asBool(), true);
}

TEST(ValueTest, NumberConstructor) {
    Value v(3.14);
    EXPECT_EQ(v.getType(), Value::VAL_NUMBER);
    EXPECT_EQ(v.asNumber(), 3.14);
}