#include "base_e2e_test_fixture.hpp"

#include "../../src/error/runtime_exception.hpp"

class NativeFunctionE2ETest : public BaseE2ETestFixture {};

TEST_F(NativeFunctionE2ETest, NativeFunctionClock) {
    // Arrange
    std::string source = "print clock();";

    // Act & Assert
    runProgramm(source);
}

TEST_F(NativeFunctionE2ETest, NativeFunctionWithArityViolation) {
    // Arrange
    std::string source = "clock(1);";

    // Act & Assert
    ASSERT_THROW(runProgrammThrowingException(source), cppLox::Error::RunTimeException);
}
