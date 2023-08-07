#include "../../src/value.hpp"
#include "gtest/gtest.h"

// Test fixture for Value unit tests
class ValueTest : public ::testing::Test {
  public:
    Value numVal;
    Value boolVal;
    Value nullVal;
    void SetUp() override {
        numVal = Value(3.14);
        boolVal = Value(true);
        nullVal = Value();
    }
};

TEST_F(ValueTest, is) {
    EXPECT_TRUE(numVal.is(Value::VAL_NUMBER));
    EXPECT_FALSE(numVal.is(Value::VAL_NULL));
    EXPECT_FALSE(numVal.is(Value::VAL_BOOL));
    EXPECT_TRUE(boolVal.is(Value::VAL_BOOL));
    EXPECT_FALSE(boolVal.is(Value::VAL_NULL));
    EXPECT_FALSE(boolVal.is(Value::VAL_NUMBER));
    EXPECT_TRUE(nullVal.is(Value::VAL_NULL));
    EXPECT_FALSE(nullVal.is(Value::VAL_BOOL));
    EXPECT_FALSE(nullVal.is(Value::VAL_NUMBER));
}

TEST_F(ValueTest, as) {
    EXPECT_EQ(numVal.asNumber(), 3.14);
    EXPECT_EQ(boolVal.asBool(), true);
}

TEST_F(ValueTest, getType) {
    EXPECT_EQ(numVal.getType(), Value::VAL_NUMBER);
    EXPECT_EQ(boolVal.getType(), Value::VAL_BOOL);
    EXPECT_EQ(nullVal.getType(), Value::VAL_NULL);
}

TEST_F(ValueTest, EqualityOperator) {
    EXPECT_TRUE(numVal == Value(3.14));
    EXPECT_FALSE(numVal == boolVal);
}

TEST_F(ValueTest, InequalityOperator) {
    EXPECT_FALSE(numVal != Value(3.14));
    EXPECT_TRUE(numVal != boolVal);
}

TEST_F(ValueTest, ExtractionOperator) {
    testing::internal::CaptureStdout();
    std::cout << numVal;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "3.14");
    testing::internal::CaptureStdout();
    std::cout << boolVal;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "true");
    testing::internal::CaptureStdout();
    std::cout << nullVal;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "null");
}

TEST_F(ValueTest, Negate) {
    EXPECT_EQ(Value(-3.14), -numVal);
}

TEST_F(ValueTest, Add) {
    EXPECT_EQ(Value(6.28), numVal + numVal);
}

TEST_F(ValueTest, AddFail) {
    ASSERT_DEATH(numVal + boolVal, "addition is only defined for numbers");
}

TEST_F(ValueTest, Subtract) {
    EXPECT_EQ(Value(0.0), numVal - numVal);
}

TEST_F(ValueTest, SubtractFail) {
    ASSERT_DEATH(numVal - boolVal, "subtraction is only defined for numbers");
}

TEST_F(ValueTest, Multiply) {
    EXPECT_EQ(Value(9.8596), numVal * numVal);
}

TEST_F(ValueTest, MultiplyFail) {
    ASSERT_DEATH(numVal * boolVal, "multiplication is only defined for numbers");
}

TEST_F(ValueTest, Divide) {
    EXPECT_EQ(Value(1.0), numVal / numVal);
}

TEST_F(ValueTest, DivideFail) {
    ASSERT_DEATH(numVal / boolVal, "division is only defined for numbers");
}