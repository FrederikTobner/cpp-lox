#include "base_e2e_test_fixture.hpp"

#include "../../src/error/runtime_exception.hpp"

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

TEST_F(UnaryOperatorE2ETest, NegateOnString) {
    // Arrange
    std::string source = "print -\"a\";";

    // Act & Assert
    ASSERT_THROW(runProgramm(source), cppLox::Error::RunTimeException);
}

TEST_F(UnaryOperatorE2ETest, NegateOnBool) {
    // Arrange
    std::string source = "print -true;";

    // Act & Assert
    ASSERT_THROW(runProgramm(source), cppLox::Error::RunTimeException);
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

TEST_F(UnaryOperatorE2ETest, NotOnNumber) {
    // Arrange
    std::string source = "print !1;";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ("false\n", output);
}

TEST_F(UnaryOperatorE2ETest, NotOnString) {
    // Arrange
    std::string source = "print !\"a\";";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ("false\n", output);
}
