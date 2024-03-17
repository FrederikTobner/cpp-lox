#include "base_e2e_test_fixture.hpp"

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

TEST_F(FunctionE2ETest, NativeFunctionClock) {
    // Arrange
    std::string source = "print clock();";

    // Act & Assert
    ASSERT_NO_THROW(runAndCaptureStdout(source));
}
