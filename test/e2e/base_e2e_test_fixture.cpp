#include "base_e2e_test_fixture.hpp"

#include "../../src/init.hpp"

std::string BaseE2ETestFixture::runAndCaptureStdout(std::string & source) {
    testing::internal::CaptureStdout();
    cppLox::run(source, *lexer, memoryMutator.get(), *compiler, *vm);
    return testing::internal::GetCapturedStdout();
}

void BaseE2ETestFixture::runProgramm(std::string & source) {
    cppLox::run(source, *lexer, memoryMutator.get(), *compiler, *vm);
}

void BaseE2ETestFixture::SetUp() {
    lexer = std::make_unique<cppLox::Frontend::Lexer>();
    memoryMutator = std::make_unique<cppLox::MemoryMutator>();
    vm = std::make_unique<cppLox::Backend::VM>(memoryMutator.get());
    compiler = std::make_unique<cppLox::Frontend::Compiler>(memoryMutator.get());
}