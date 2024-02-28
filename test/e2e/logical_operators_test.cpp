#include "base_e2e_test_fixture.hpp"

#include <format>

class LogicalOperatorsParameterizedE2ETestFixture : public BaseE2ETestFixture,
                                                    public ::testing::WithParamInterface<std::pair<bool, bool>> {};

INSTANTIATE_TEST_SUITE_P(LogicalOperatorE2ETest, LogicalOperatorsParameterizedE2ETestFixture,
                         ::testing::Values(std::make_pair(true, true), std::make_pair(false, false),
                                           std::make_pair(true, false), std::make_pair(false, true)));

TEST_P(LogicalOperatorsParameterizedE2ETestFixture, OrOperator) {
    // Arrange
    auto [left, right] = GetParam();
    std::string source = std::format("print {} or {};", left, right);

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(std::format("{}\n", left || right), output);
}

TEST_P(LogicalOperatorsParameterizedE2ETestFixture, AndOperator) {
    // Arrange
    auto [left, right] = GetParam();
    std::string source = std::format("print {} and {};", left, right);

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(std::format("{}\n", left && right), output);
}
