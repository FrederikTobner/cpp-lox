#include <gtest/gtest.h>

#include <memory>
#include <string_view>

#include "../../src/backend/vm.hpp"
#include "../../src/frontend/compiler.hpp"
#include "../../src/frontend/lexer.hpp"
#include "../../src/frontend/token.hpp"
#include "../../src/memory_mutator.hpp"

class BaseE2ETestFixture : public ::testing::Test {
  protected:
    auto SetUp() -> void override;
    auto runAndCaptureStdout(std::string & source) -> std::string;
    auto runProgramm(std::string & source) -> void;

  private:
    std::unique_ptr<cppLox::Frontend::Lexer> lexer;
    std::shared_ptr<cppLox::MemoryMutator> memoryMutator;
    std::unique_ptr<cppLox::Frontend::Compiler> compiler;
    std::unique_ptr<cppLox::Backend::VM> vm;
};