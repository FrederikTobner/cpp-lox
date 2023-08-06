#include "../../src/value.hpp"
#include "gtest/gtest.h"

// Test fixture for Value unit tests
class ValueTest : public ::testing::Test {
  public:
    Value v;
    Value v2;
    Value v3;
    Value v4;
    void SetUp() override {
        v = Value(3.14);
        v2 = Value(3.14);
        v3 = Value(true);
        v4 = Value();
    }
};

TEST_F(ValueTest, is) {
    EXPECT_TRUE(v.is(Value::VAL_NUMBER));
    EXPECT_FALSE(v.is(Value::VAL_NULL));
    EXPECT_FALSE(v.is(Value::VAL_BOOL));
    EXPECT_TRUE(v3.is(Value::VAL_BOOL));
    EXPECT_FALSE(v3.is(Value::VAL_NULL));
    EXPECT_FALSE(v3.is(Value::VAL_NUMBER));
    EXPECT_TRUE(v4.is(Value::VAL_NULL));
    EXPECT_FALSE(v4.is(Value::VAL_BOOL));
    EXPECT_FALSE(v4.is(Value::VAL_NUMBER));
}

TEST_F(ValueTest, EqualityOperator) {
    EXPECT_TRUE(v == v2);
    EXPECT_FALSE(v == v3);
}

TEST_F(ValueTest, InequalityOperator) {
    EXPECT_FALSE(v != v2);
    EXPECT_TRUE(v != v3);
}

TEST_F(ValueTest, ExtractionOperator) {
    testing::internal::CaptureStdout();
    std::cout << v;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "3.14");
}

TEST_F(ValueTest, Negate) {
    Value v5 = -v;
    EXPECT_EQ(Value(-3.14), v5);
}

TEST_F(ValueTest, Add) {
    Value v5 = v + v2;
    EXPECT_EQ(Value(6.28), v5);
}

TEST_F(ValueTest, Subtract) {
    Value v5 = v - v2;
    EXPECT_EQ(Value(0.0), v5);
}

TEST_F(ValueTest, Multiply) {
    Value v5 = v * v2;
    EXPECT_EQ(Value(9.8596), v5);
}

TEST_F(ValueTest, Divide) {
    Value v5 = v / v2;
    EXPECT_EQ(Value(1.0), v5);
}