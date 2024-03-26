#include "base_e2e_test_fixture.hpp"

#include "../../src/init.hpp"

auto BaseE2ETestFixture::runAndCaptureStdout(std::string & source) -> std::string {
    testing::internal::CaptureStdout();
    runProgramm(source);
    return testing::internal::GetCapturedStdout();
}

auto BaseE2ETestFixture::runProgramm(std::string & source) -> void {
    EXPECT_NO_THROW(cppLox::run(source, *lexer, *compiler, *vm)) << "Running the programm resulted in an exception";
}

auto BaseE2ETestFixture::runProgrammThrowingException(std::string & source) -> void {
    cppLox::run(source, *lexer, *compiler, *vm);
}

auto BaseE2ETestFixture::SetUp() -> void {
    lexer = std::make_unique<cppLox::Frontend::Lexer>();
    memoryMutator = std::make_shared<cppLox::MemoryMutator>();
    vm = std::make_unique<cppLox::Backend::VM>(memoryMutator);
    compiler = std::make_unique<cppLox::Frontend::Compiler>(memoryMutator);
}