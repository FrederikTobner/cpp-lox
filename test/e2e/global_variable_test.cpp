#include "base_e2e_test_fixture.hpp"

#include "../../src/error/runtime_exception.hpp"

class GlobalVariableE2ETest : public BaseE2ETestFixture {};

TEST_F(GlobalVariableE2ETest, Assignment) {
    // Arrange
    std::string source = "var a; a = 1; print a;";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(GlobalVariableE2ETest, AssignmentToUndefinedVariable) {
    // Arrange
    std::string source = "a = 1;";
    std::string expected = "1\n";

    // Act & Assert
    ASSERT_THROW(runProgramm(source), cppLox::Error::RunTimeException);
}

TEST_F(GlobalVariableE2ETest, Declaration) {
    // Arrange
    std::string source = "var a; print a;";
    std::string expected = "null\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(GlobalVariableE2ETest, DeclarationAndAssignmentStatement) {
    // Arrange
    std::string source = "var a = 1; print a;";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
