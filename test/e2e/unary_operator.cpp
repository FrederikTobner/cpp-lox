#include "base_e2e_test_fixture.hpp"

class UnaryOperatorE2ETest : public BaseE2ETestFixture {};

TEST_F(UnaryOperatorE2ETest, Negate) {
    // Arrange
    std::string source = "var i = -1; print -i;";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(UnaryOperatorE2ETest, Not) {
    // Arrange
    std::string source = "print !true;";
    std::string expected = "false\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
