#include "base_e2e_test_fixture.hpp"

class BinaryOperatorE2ETest : public BaseE2ETestFixture {};

TEST_F(BinaryOperatorE2ETest, Addition) {
    // Arrange
    std::string source = "print 1 + 2;";
    std::string expected = "3\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, StringConcatenation) {
    // Arrange
    std::string source = "print \"a\" + \"b\";";
    std::string expected = "ab\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Subtraction) {
    // Arrange
    std::string source = "print 1 - 2;";
    std::string expected = "-1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Multiplication) {
    // Arrange
    std::string source = "print 1 * 2;";
    std::string expected = "2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Division) {
    // Arrange
    std::string source = "print 1 / 2;";
    std::string expected = "0.5\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Greater) {
    // Arrange
    std::string source = "print 1 > 2;";
    std::string expected = "false\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, GreaterEqual) {
    // Arrange
    std::string source = "print 1 >= 2;";
    std::string expected = "false\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Less) {
    // Arrange
    std::string source = "print 1 < 2;";
    std::string expected = "true\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, LessEqual) {
    // Arrange
    std::string source = "print 1 <= 2;";
    std::string expected = "true\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, Equal) {
    // Arrange
    std::string source = "print 1 == 2;";
    std::string expected = "false\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(BinaryOperatorE2ETest, NotEqual) {
    // Arrange
    std::string source = "print 1 != 2;";
    std::string expected = "true\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}