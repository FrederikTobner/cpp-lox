#include "base_e2e_test_fixture.hpp"

class IfElseStatementE2ETest : public BaseE2ETestFixture {};

TEST_F(IfElseStatementE2ETest, TruthyIfStatement) {
    // Arrange
    std::string source = "if (true) print 1;";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(IfElseStatementE2ETest, FalsyIfStatement) {
    // Arrange
    std::string source = "if (false) print 1;";
    std::string expected = "";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(IfElseStatementE2ETest, TruthyElseStatement) {
    // Arrange
    std::string source = "if (false) print 1; else print 2;";
    std::string expected = "2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(IfElseStatementE2ETest, FalsyElseStatement) {
    // Arrange
    std::string source = "if (true) print 1; else print 2;";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
