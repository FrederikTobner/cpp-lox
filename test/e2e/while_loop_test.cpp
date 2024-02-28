#include "base_e2e_test_fixture.hpp"

class WhileLoopE2ETest : public BaseE2ETestFixture {};

TEST_F(WhileLoopE2ETest, SimpleWhileLoop) {
    // Arrange
    std::string source = "var i = 0; while (i < 3) { print i; i = i + 1; }";
    std::string expected = "0\n1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
