#include "compilation_scope.hpp"

using namespace cppLox::Frontend;

CompilationScope::CompilationScope() : m_localCount(0) {
}

CompilationScope::CompilationScope(std::shared_ptr<CompilationScope> enclosing)
    : m_enclosing(enclosing), m_localCount(0) {
}

auto CompilationScope::enclosing() const -> std::optional<std::shared_ptr<CompilationScope>> const & {
    return m_enclosing;
}

auto CompilationScope::local(uint16_t index) const -> Local const & {
    return *m_locals[index];
}

auto CompilationScope::localCount() const -> uint16_t const & {
    return m_localCount;
}

auto CompilationScope::addLocal(cppLox::Frontend::Token name, std::function<void(std::string &)> onError) -> void {
    if (m_localCount == 256) {
        std::string message = "Too many local variables in function";
        onError(message);
    }
    m_locals[m_localCount] = std::make_unique<Local>(name, -1);
    m_localCount++;
}

auto CompilationScope::markInitialized(uint16_t depth) -> void {
    m_locals[m_localCount - 1]->m_depth = depth;
}

auto CompilationScope::popLocal() -> void {
    m_localCount--;
}