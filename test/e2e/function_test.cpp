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