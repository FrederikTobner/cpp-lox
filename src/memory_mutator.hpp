#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "types/object.hpp"

/// @brief The memory manager that manages all objects.
class MemoryMutator {

  public:
    MemoryMutator() = default;
    ~MemoryMutator() = default;
    /// @brief Creates a new object of the given type.
    /// @tparam ...Args The types of the arguments to pass to the constructor.
    /// @tparam T The type of the object to create.
    /// @param ...args The arguments to pass to the constructor.
    /// @return A pointer to the newly created object.
    template <typename T, class... Args> auto create(Args &&... args) -> cppLox::Types::Object * {
        cppLox::Types::Object * object = static_cast<cppLox::Types::Object *>(new T(std::forward<Args>(args)...));
        m_objects.push_back(std::unique_ptr<cppLox::Types::Object>(object));
        return object;
    }

  private:
    /// @brief The list of all objects that are currently allocated.
    std::vector<std::unique_ptr<cppLox::Types::Object>> m_objects;
};