#pragma once

#include "object.hpp"
#include "value.hpp"

namespace cppLox::Types {

/// @brief Represents an upvalue object.
class ObjectUpValue : public Object {
  public:
    /// @brief Constructs a new upvalue object.
    /// @param closed The value that is closed.
    ObjectUpValue(Value * closed);

    /// @brief Destructor of the upvalue object.
    ~ObjectUpValue() override = default;

    /// @brief Gets the closed value.
    /// @return The closed value.
    [[nodiscard]] auto closed() const noexcept -> Value *;

    /// @brief Sets the closed value.
    /// @param closed The closed value.
    auto setClosed(Value * closed) -> void;

    /// @brief Writes the upvalue to the output stream.
    /// @param os The output stream to write to.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << "upvalue";
    }

  private:
    /// @brief The enclosed value.
    Value * m_closed;
};

} // namespace cppLox::Types