#include "base_e2e_test_fixture.hpp"

#include "../../src/error/runtime_exception.hpp"

class LocalVariableE2ETest : public BaseE2ETestFixture {};

TEST_F(LocalVariableE2ETest, Assignment) {
    // Arrange
    std::string source = "{var a; a = 1; print a;}";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(LocalVariableE2ETest, AssignmentToUndefinedVariable) {
    // Arrange
    std::string source = "{a = 1;}";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}

TEST_F(LocalVariableE2ETest, Declaration) {
    // Arrange
    std::string source = "{var a; print a;}";
    std::string expected = "null\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(LocalVariableE2ETest, DeclarationAndAssignmentStatement) {
    // Arrange
    std::string source = "{var a = 1; print a;}";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(LocalVariableE2ETest, SameNameWithinDifferentScopesDoNotClash) {
    // Arrange
    std::string source = "{var a; a = 1; print a;} {var a; a = 2; print a;}";
    std::string expected = "1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(LocalVariableE2ETest, CanGetLocalVariableFromOuterScopeWithOtherVariables) {
    // Arrange
    std::string source = "{var a = 1; var b = 2; { var c = 3; var d = 4; var e = 5; print a; }}";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(LocalVariableE2ETest, CanGetLocalVariableFromOuterScopeWithoutOtherVariables) {
    // Arrange
    std::string source = "{var a = 1; { print a; }}";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}
