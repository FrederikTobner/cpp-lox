#include "../../src/runtime_exception.hpp"
#include "../../src/value.hpp"

#include <gtest/gtest.h>

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
    ASSERT_EQ(Value(-3.14), -numVal);
}

TEST_F(ValueTest, Add) {
    ASSERT_EQ(Value(6.28), numVal + numVal);
}

TEST_F(ValueTest, AddFail) {
    ASSERT_THROW(numVal + boolVal, RunTimeException);
}

TEST_F(ValueTest, Subtract) {
    ASSERT_EQ(Value(0.0), numVal - numVal);
}

TEST_F(ValueTest, SubtractFail) {
    ASSERT_THROW(numVal - boolVal, RunTimeException);
}

TEST_F(ValueTest, Multiply) {
    ASSERT_EQ(Value(9.8596), numVal * numVal);
}

TEST_F(ValueTest, MultiplyFail) {
    ASSERT_THROW(numVal * boolVal, RunTimeException);
}

TEST_F(ValueTest, Divide) {
    ASSERT_EQ(Value(1.0), numVal / numVal);
}

TEST_F(ValueTest, DivideFail) {
    ASSERT_THROW(numVal / boolVal, RunTimeException);
}

TEST_F(ValueTest, Greater) {
    ASSERT_EQ(Value(true), numVal > Value(2.0));
}

TEST_F(ValueTest, GreaterFail) {
    ASSERT_THROW(numVal > boolVal, RunTimeException);
}

TEST_F(ValueTest, GreaterEqual) {
    ASSERT_EQ(Value(true), numVal >= Value(3.14));
}

TEST_F(ValueTest, GreaterEqualFail) {
    ASSERT_THROW(numVal >= boolVal, RunTimeException);
}

TEST_F(ValueTest, Less) {
    ASSERT_EQ(Value(true), numVal < Value(4.0));
}

TEST_F(ValueTest, LessFail) {
    ASSERT_THROW(numVal < boolVal, RunTimeException);
}

TEST_F(ValueTest, LessEqual) {
    ASSERT_EQ(Value(true), numVal <= Value(3.14));
}

TEST_F(ValueTest, LessEqualFail) {
    ASSERT_THROW(numVal <= boolVal, RunTimeException);
}
