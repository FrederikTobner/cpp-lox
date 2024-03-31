#include "base_e2e_test_fixture.hpp"

#include "../../src/error/runtime_exception.hpp"

class FunctionE2ETest : public BaseE2ETestFixture {};

TEST_F(FunctionE2ETest, PrintFunction) {
    // Arrange
    std::string source = "fun foo() { print 1; } print foo;";
    std::string expected = "<fn foo>\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, CallFunction) {
    // Arrange
    std::string source = "fun foo() { print 1; } foo(); print 2;";
    std::string expected = "1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, FunctionArguments) {
    // Arrange
    std::string source = "fun argFun(a, b) { print a; print b; } argFun(1, 2);";
    std::string expected = "1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, FunctionReturn) {
    // Arrange
    std::string source = "fun returnFun() { return 1; } print returnFun();";
    std::string expected = "1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, CallString) {
    // Arrange
    std::string source = R"(var a = \"foo\"; a();)";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}

TEST_F(FunctionE2ETest, CallNumber) {
    // Arrange
    std::string source = R"(var a = 1; a();)";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}

TEST_F(FunctionE2ETest, CallBool) {
    // Arrange
    std::string source = R"(var a = true; a();)";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}

TEST_F(FunctionE2ETest, CallNull) {
    // Arrange
    std::string source = R"(var a = null; a();)";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}
