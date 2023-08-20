#include "../../src/error/runtime_exception.hpp"
#include "../../src/types/value.hpp"
#include "../../src/types/value_formatter.hpp"

#include <gtest/gtest.h>

Value numVal = Value(3.14);
Value boolVal = Value(true);
Value nullVal = Value();
Value objectVal = Value((Object *)nullptr);

TEST(ValueTest, is) {
    EXPECT_TRUE(numVal.is(Value::Type::NUMBER));
    EXPECT_FALSE(numVal.is(Value::Type::NULL_));
    EXPECT_FALSE(numVal.is(Value::Type::BOOL));
    EXPECT_TRUE(boolVal.is(Value::Type::BOOL));
    EXPECT_FALSE(boolVal.is(Value::Type::NULL_));
    EXPECT_FALSE(boolVal.is(Value::Type::NUMBER));
    EXPECT_TRUE(nullVal.is(Value::Type::NULL_));
    EXPECT_FALSE(nullVal.is(Value::Type::BOOL));
    EXPECT_FALSE(nullVal.is(Value::Type::NUMBER));
}

TEST(ValueTest, as) {
    EXPECT_EQ(numVal.as<double>(), 3.14);
    EXPECT_EQ(boolVal.as<bool>(), true);
    EXPECT_EQ(nullVal.as<Object *>(), nullptr);
}

TEST(ValueTest, getType) {
    EXPECT_EQ(numVal.getType(), Value::Type::NUMBER);
    EXPECT_EQ(boolVal.getType(), Value::Type::BOOL);
    EXPECT_EQ(nullVal.getType(), Value::Type::NULL_);
}

TEST(ValueTest, EqualityOperator) {
    EXPECT_TRUE(numVal == Value(3.14));
    EXPECT_FALSE(numVal == boolVal);
}

TEST(ValueTest, InequalityOperator) {
    EXPECT_FALSE(numVal != Value(3.14));
    EXPECT_TRUE(numVal != boolVal);
}

TEST(ValueTest, ExtractionOperator) {
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

TEST(ValueTest, Formatter) {
    // Formatting objects is tested in test\unit\object_test.cpp
    EXPECT_EQ(std::format("{}", numVal), "3.14");
    EXPECT_EQ(std::format("{}", boolVal), "true");
    EXPECT_EQ(std::format("{}", nullVal), "null");
}

TEST(ValueTest, Negate) {
    ASSERT_EQ(Value(-3.14), -numVal);
}

TEST(ValueTest, Add) {
    ASSERT_EQ(Value(6.28), numVal + numVal);
}

TEST(ValueTest, AddFail) {
    ASSERT_THROW({ Value val = numVal + boolVal; }, RunTimeException);
}

TEST(ValueTest, Subtract) {
    ASSERT_EQ(Value(0.0), numVal - numVal);
}

TEST(ValueTest, SubtractFail) {
    ASSERT_THROW({ Value val = numVal - boolVal; }, RunTimeException);
}

TEST(ValueTest, Multiply) {
    ASSERT_EQ(Value(9.8596), numVal * numVal);
}

TEST(ValueTest, MultiplyFail) {
    ASSERT_THROW({ Value val = numVal * boolVal; }, RunTimeException);
}

TEST(ValueTest, Divide) {
    ASSERT_EQ(Value(1.0), numVal / numVal);
}

TEST(ValueTest, DivideFail) {
    ASSERT_THROW({ Value val = numVal / boolVal; }, RunTimeException);
}

TEST(ValueTest, Greater) {
    ASSERT_EQ(Value(true), numVal > Value(2.0));
}

TEST(ValueTest, GreaterFail) {
    ASSERT_THROW({ Value val = numVal > boolVal; }, RunTimeException);
}

TEST(ValueTest, GreaterEqual) {
    ASSERT_EQ(Value(true), numVal >= Value(3.14));
}

TEST(ValueTest, GreaterEqualFail) {
    ASSERT_THROW({ Value val = numVal >= boolVal; }, RunTimeException);
}

TEST(ValueTest, Less) {
    ASSERT_EQ(Value(true), numVal < Value(4.0));
}

TEST(ValueTest, LessFail) {
    ASSERT_THROW({ Value val = numVal < boolVal; }, RunTimeException);
}

TEST(ValueTest, LessEqual) {
    ASSERT_EQ(Value(true), numVal <= Value(3.14));
}

TEST(ValueTest, LessEqualFail) {
    ASSERT_THROW({ Value val = numVal <= boolVal; }, RunTimeException);
}
