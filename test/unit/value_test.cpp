#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "../../src/types/value.hpp"

#include "../../src/error/runtime_exception.hpp"
#include "../../src/types/value_formatter.hpp"

class ValueTest : public ::testing::Test {
  protected:
    ValueTest();

    cppLox::Types::Value numVal;
    cppLox::Types::Value boolVal;
    cppLox::Types::Value nullVal;
    cppLox::Types::Value objectVal;
};

ValueTest::ValueTest()
    : numVal(3.14), boolVal(true), nullVal(), objectVal(static_cast<cppLox::Types::Object *>(nullptr)) {
}

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

TEST_F(ValueTest, Add) {
    ASSERT_EQ(cppLox::Types::Value(6.28), numVal + numVal);
}

TEST_F(ValueTest, AddWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal + boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal + numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, AddWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal + nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal + numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, AddWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal + objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal + numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Divide) {
    ASSERT_EQ(cppLox::Types::Value(1.0), numVal / numVal);
}

TEST_F(ValueTest, DivideWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal / boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal / numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, DivideWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal / nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal / numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, DivideWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal / objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal / numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Greater) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal > cppLox::Types::Value(2.0));
}

TEST_F(ValueTest, GreaterWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal > boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal > numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal > nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal > numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal > objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal > numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterEqual) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal >= cppLox::Types::Value(3.14));
}

TEST_F(ValueTest, GreaterEqualWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal >= boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal >= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterEqualWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal >= nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal >= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, GreaterEqualWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal >= objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal >= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Less) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal < cppLox::Types::Value(4.0));
}

TEST_F(ValueTest, LessWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal < boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal < numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal < nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal < numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal < objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal < numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessEqual) {
    ASSERT_EQ(cppLox::Types::Value(true), numVal <= cppLox::Types::Value(3.14));
}

TEST_F(ValueTest, LessEqualWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal <= boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal <= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessEqualWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal <= nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal <= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, LessEqualWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal <= objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal <= numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Multiply) {
    ASSERT_EQ(cppLox::Types::Value(9.8596), numVal * numVal);
}

TEST_F(ValueTest, MultiplyWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal * boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal * numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, MultiplyWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal * nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal * numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, MultiplyWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal * objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal * numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Negate) {
    ASSERT_EQ(cppLox::Types::Value(-3.14), -numVal);
}

TEST_F(ValueTest, NegateWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = -boolVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, NegateWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = -nullVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, NegateWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = -objectVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, Not) {
    ASSERT_EQ(cppLox::Types::Value(false), !boolVal);
}

TEST_F(ValueTest, Subtract) {
    ASSERT_EQ(cppLox::Types::Value(0.0), numVal - numVal);
}

TEST_F(ValueTest, SubtractWithBoolean) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal - boolVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = boolVal - numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, SubtractWithNull) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal - nullVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = nullVal - numVal; }, cppLox::Error::RunTimeException);
}

TEST_F(ValueTest, SubtractWithObject) {
    ASSERT_THROW({ cppLox::Types::Value val = numVal - objectVal; }, cppLox::Error::RunTimeException);
    ASSERT_THROW({ cppLox::Types::Value val = objectVal - numVal; }, cppLox::Error::RunTimeException);
}

class ValuePrintedTest : public ::testing::TestWithParam<std::pair<cppLox::Types::Value, std::string>> {};

INSTANTIATE_TEST_SUITE_P(Value, ValuePrintedTest,
                         ::testing::Values(std::make_pair(cppLox::Types::Value(3.14), "3.14"),
                                           std::make_pair(cppLox::Types::Value(true), "true"),
                                           std::make_pair(cppLox::Types::Value(), "null")));

TEST_P(ValuePrintedTest, ExtractionOperator) {
    auto [value, expected] = GetParam();
    std::ostringstream oss;
    oss << value;
    EXPECT_EQ(oss.str(), expected);
}

TEST_P(ValuePrintedTest, Formatter) {
    auto [value, expected] = GetParam();
    EXPECT_EQ(std::format("{}", value), expected);
}

TEST(ValuePrintedTest, ExtractionOperatorOnObject) {
    auto stringObj = std::make_unique<cppLox::Types::ObjectString>("test");
    cppLox::Types::Value value(static_cast<cppLox::Types::Object *>(stringObj.get()));
    std::ostringstream oss;
    oss << value;
    EXPECT_EQ(oss.str(), "test");
}

TEST(ValuePrintedTest, FormatObject) {
    auto stringObj = std::make_unique<cppLox::Types::ObjectString>("test");
    cppLox::Types::Value value(static_cast<cppLox::Types::Object *>(stringObj.get()));
    EXPECT_EQ(std::format("{}", value), "test");
}