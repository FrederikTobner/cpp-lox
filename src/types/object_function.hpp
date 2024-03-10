#pragma once

#include <memory>

#include "../bytecode/chunk.hpp"
#include "object.hpp"
#include "object_string.hpp"

namespace cppLox::Types {

/// @brief Represents a callable function object.
class ObjectFunction : public Object {
  public:
    ObjectFunction(uint16_t arity, ObjectString * name) : m_arity(arity), m_name(name) {
        m_type = Object::Type::FUNCTION;
        m_chunk = std::make_unique<cppLox::ByteCode::Chunk>();
    }

    ~ObjectFunction() override = default;

    auto operator=(ObjectFunction const & other) -> ObjectFunction & {
        m_arity = other.m_arity;
        m_chunk = std::make_unique<cppLox::ByteCode::Chunk>(*other.m_chunk);
        m_name = other.m_name;
        return *this;
    };

    /// @brief Gets the arity of the function.
    /// @return The arity of the function.
    [[nodiscard]] auto arity() const -> uint16_t {
        return m_arity;
    }

    /// @brief Gets the chunk of the function.
    /// @return The chunk of the function.
    [[nodiscard]] auto chunk() -> cppLox::ByteCode::Chunk * {
        return m_chunk.get();
    }

    /// @brief Gets the name of the function.
    /// @return The name of the function.
    [[nodiscard]] auto name() const -> ObjectString * {
        return m_name;
    }

    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << m_name->string();
    }

  private:
    uint16_t m_arity;
    std::unique_ptr<cppLox::ByteCode::Chunk> m_chunk;
    ObjectString * m_name;
};

} // namespace cppLox::Types