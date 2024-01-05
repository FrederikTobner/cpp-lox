#include "../../src/error/runtime_exception.hpp"
#include "../../src/types/value.hpp"
#include "../../src/types/value_formatter.hpp"

#include <gtest/gtest.h>

class ValueTest : public ::testing::Test {
  public:
    cppLox::Types::Value numVal;
    cppLox::Types::Value boolVal;
    cppLox::Types::Value nullVal;
    cppLox::Types::Value objectVal;

    ValueTest() {
        numVal = cppLox::Types::Value(3.14);
        boolVal = cppLox::Types::Value(true);
        nullVal = cppLox::Types::Value();
        objectVal = cppLox::Types::Value((cppLox::Types::Object *)nullptr);
    }
};

TEST_F(ValueTest, is) {
    // Act
    EXPECT_TRUE(numVal.is(cppLox::Types::Value::Type::NUMBER));
    EXPECT_FALSE(numVal.is(cppLox::Types::Value::Type::NULL_));
    EXPECT_FALSE(numVal.is(cppLox::Types::Value::Type::BOOL));
    EXPECT_FALSE(numVal.is(cppLox::Types::Value::Type::OBJECT));

    EXPECT_TRUE(boolVal.is(cppLox::Types::Value::Type::BOOL));
    EXPECT_FALSE(boolVal.is(cppLox::Types::Value::Type::NULL_));
    EXPECT_FALSE(boolVal.is(cppLox::Types::Value::Type::NUMBER));
    EXPECT_FALSE(boolVal.is(cppLox::Types::Value::Type::OBJECT));

    EXPECT_TRUE(nullVal.is(cppLox::Types::Value::Type::NULL_));
    EXPECT_FALSE(nullVal.is(cppLox::Types::Value::Type::BOOL));
    EXPECT_FALSE(nullVal.is(cppLox::Types::Value::Type::NUMBER));
    EXPECT_FALSE(nullVal.is(cppLox::Types::Value::Type::OBJECT));

    EXPECT_TRUE(objectVal.is(cppLox::Types::Value::Type::OBJECT));
    EXPECT_FALSE(objectVal.is(cppLox::Types::Value::Type::BOOL));
    EXPECT_FALSE(objectVal.is(cppLox::Types::Value::Type::NUMBER));
    EXPECT_FALSE(objectVal.is(cppLox::Types::Value::Type::NULL_));
}

TEST_F(ValueTest, as) {
    EXPECT_EQ(numVal.as<double>(), 3.14);
    EXPECT_EQ(boolVal.as<bool>(), true);
    EXPECT_EQ(objectVal.as<cppLox::Types::Object *>(), nullptr);
}

TEST_F(ValueTest, getType) {
    EXPECT_EQ(numVal.getType(), cppLox::Types::Value::Type::NUMBER);
    EXPECT_EQ(boolVal.getType(), cppLox::Types::Value::Type::BOOL);
    EXPECT_EQ(nullVal.getType(), cppLox::Types::Value::Type::NULL_);
}

TEST_F(ValueTest, EqualityOperator) {
    EXPECT_TRUE(numVal == cppLox::Types::Value(3.14));
    EXPECT_FALSE(numVal == boolVal);
}

TEST_F(ValueTest, InequalityOperator) {
    EXPECT_FALSE(numVal != cppLox::Types::Value(3.14));
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

TEST_F(ValueTest, Formatter) {
    EXPECT_EQ(std::format("{}", numVal), "3.14");
    EXPECT_EQ(std::format("{}", boolVal), "true");
    EXPECT_EQ(std::format("{}", nullVal), "null");
}

TEST_F(ValueTest, Negate) {
    ASSERT_EQ(cppLox::Types::Value(-3.14), -numVal);
}

TEST_F(ValueTest, Add) {
    ASSERT_EQ(cppLox::Types::Value(6.28), numVal + numVal);
}

TEST_F(ValueTest, AddFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal + boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Divide) {
    ASSERT_EQ(cppLox::Types::Value(1.0), numVal / numVal);
}

TEST_F(ValueTest, DivideFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal / boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Greater) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal > cppLox::Types::Value(2.0));
}

TEST_F(ValueTest, GreaterFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal > boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterEqual) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal >= cppLox::Types::Value(3.14));
}

TEST_F(ValueTest, GreaterEqualFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal >= boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Less) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal < cppLox::Types::Value(4.0));
}

TEST_F(ValueTest, LessFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal < boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessEqual) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal <= cppLox::Types::Value(3.14));
}

TEST_F(ValueTest, LessEqualFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal <= boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Multiply) {
    ASSERT_EQ(cppLox::Types::Value(9.8596), numVal * numVal);
}

TEST_F(ValueTest, MultiplyFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal * boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Subtract) {
    ASSERT_EQ(cppLox::Types::Value(0.0), numVal - numVal);
}

TEST_F(ValueTest, SubtractFail) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal - boolVal; }, cppLox::Error::RunTimeException);
}
