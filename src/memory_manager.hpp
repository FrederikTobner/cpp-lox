#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "object.hpp"

class MemoryManager {

  public:
    MemoryManager() = default;
    ~MemoryManager() = default;
    /// @brief Creates a new object of the given type.
    /// @tparam ...Args The types of the arguments to pass to the constructor.
    /// @tparam T The type of the object to create.
    /// @param ...args The arguments to pass to the constructor.
    /// @return A pointer to the newly created object.
    template <typename T, class... Args> auto create(Args &&... args) -> Object * {
        Object * object = static_cast<Object *>(new T(std::forward<Args>(args)...));
        m_objects.push_back(std::unique_ptr<Object>(object));
        return object;
    }

    /// Concatenates the given strings.
    /// @param left The left string.
    /// @param right The right string.
    /// @return The concatenated string.
    auto concatenate(ObjectString * left, ObjectString * right) -> Object * {
        return create<ObjectString>(left->string() + right->string());
    }

  private:
    /// @brief The list of all objects that are currently allocated.
    std::vector<std::unique_ptr<Object>> m_objects;
    /// @brief The mutex to lock the memory manager, to make sure no new objects are created while the garbage collector
    /// is running.
    std::mutex m_mutex;
};