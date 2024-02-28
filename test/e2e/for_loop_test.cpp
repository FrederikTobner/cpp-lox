#include "base_e2e_test_fixture.hpp"

class ForLoopE2ETest : public BaseE2ETestFixture {};

TEST_F(ForLoopE2ETest, SimpleForLoop) {
    // Arrange
    std::string source = "for (var i = 0; i < 3; i = i + 1) { print i; }";
    std::string expected = "0\n1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(ForLoopE2ETest, ForLoopWithoutInitializer) {
    // Arrange
    std::string source = "var i = 0; for (; i < 3; i = i + 1) { print i; }";
    std::string expected = "0\n1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(ForLoopE2ETest, ForLoopWithoutIncrement) {
    // Arrange
    std::string source = " for (var i = 0; i < 3; ) { print i; i = i + 1; }";
    std::string expected = "0\n1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
