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

TEST_F(FunctionE2ETest, NestedFunction)
{
    // Arrange
    std::string source = R"(
        fun outer() { 
            fun inner() { 
                print "Hello from inner!"; 
            } 
            inner(); 
        } 
        outer();
    )";
    std::string expected = "Hello from inner!\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, FunctionWithClosure)
{
    // Arrange
    std::string source = R"(
        fun makeCounter() {
            var count = 0;
            fun counter() {
                count = count + 1;
                print count;
            }
            return counter;
        }
        var myCounter = makeCounter();
        myCounter();
        myCounter();
    )";
    std::string expected = "1\n2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, ClosuresFromSeparateCallsAreIndependent)
{
    // Arrange - each call to makeCounter() must capture its own "count" local.
    std::string source = R"(
        fun makeCounter() {
            var count = 0;
            fun counter() {
                count = count + 1;
                print count;
            }
            return counter;
        }
        var counterA = makeCounter();
        var counterB = makeCounter();
        counterA();
        counterA();
        counterB();
    )";
    std::string expected = "1\n2\n1\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, MultipleClosuresShareTheSameUpvalue)
{
    // Arrange - "getter" and "setter" both capture the same "shared" local. Mutating it through
    // one closure must be visible through the other, proving the upvalue is shared and not copied.
    std::string source = R"(
        fun outer() {
            var shared = 0;
            fun getter() {
                print shared;
            }
            fun setter() {
                shared = shared + 1;
            }
            setter();
            setter();
            getter();
        }
        outer();
    )";
    std::string expected = "2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, ClosureCapturesAcrossMultipleNestingLevels)
{
    // Arrange - "innermost" does not reference "x" directly through its immediate enclosing
    // function, it must be threaded through as a non-local upvalue by "middle".
    std::string source = R"(
        fun outer() {
            var x = "outer value";
            fun middle() {
                fun innermost() {
                    print x;
                }
                innermost();
            }
            middle();
        }
        outer();
    )";
    std::string expected = "outer value\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, ClosureCapturesBlockScopedLocalThatOutlivesTheBlock)
{
    // Arrange - "local" is scoped to the inner block and must be closed over (CLOSE_UPVALUE)
    // when the block ends, so the returned closure still observes the correct value afterwards.
    std::string source = R"(
        fun makeClosure() {
            var closure;
            {
                var local = "block local";
                fun captureLocal() {
                    print local;
                }
                closure = captureLocal;
            }
            return closure;
        }
        var c = makeClosure();
        c();
    )";
    std::string expected = "block local\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}

TEST_F(FunctionE2ETest, ClosuresCreatedInLoopEachCaptureTheirOwnVariable)
{
    // Arrange - each loop iteration creates a fresh closure, stores the latest one, and the
    // returned closure still observes the final loop value after the loop has finished.
    std::string source = R"(
        fun makeClosure(value) {
            fun captureCurrent() {
                print value;
            }
            return captureCurrent;
        }

        var closure;
        var i = 0;
        while (i < 3) {
            closure = makeClosure(i);
            i = i + 1;
        }
        closure();
    )";
    std::string expected = "2\n";

    // Act
    std::string output = runAndCaptureStdout(source);

    // Assert
    ASSERT_EQ(expected, output);
}