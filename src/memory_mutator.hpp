#pragma once

#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

#include "types/object.hpp"
#include "types/object_string.hpp"

#include "simple_comperator.hpp"
#include "string_hash.hpp"

/// @brief The memory manager that manages the creation and deletion of all objects.
class MemoryMutator {

  public:
    MemoryMutator() = default;
    ~MemoryMutator() {
    }

    /// @brief Creates a new object of the given type.
    /// @tparam ...Args The types of the arguments to pass to the constructor.
    /// @tparam T The type of the object to create.
    /// @param ...args The arguments to pass to the constructor.
    /// @return A pointer to the newly created object.
    template <cppLox::Types::DerivedFromObject T, class... Args>
    auto create(Args &&... args) -> cppLox::Types::Object * {
        cppLox::Types::Object * object;
        if constexpr (std::is_same_v<T, cppLox::Types::ObjectString>) {
            auto string = new T(std::forward<Args>(args)...);
            std::unordered_set<cppLox::Types::ObjectString *>::iterator iterator = this->m_strings.find(string);
            if (iterator != m_strings.end()) {
                delete string;
                return static_cast<cppLox::Types::Object *>(*iterator);
            }
            m_strings.insert(string);
            object = static_cast<cppLox::Types::Object *>(string);
        } else {
            cppLox::Types::Object * object = static_cast<cppLox::Types::Object *>(new T(std::forward<Args>(args)...));
        }
        m_objects.push_back(std::unique_ptr<cppLox::Types::Object>(object));
        return object;
    }

  private:
    /// @brief The list of all objects that are currently allocated.
    std::vector<std::unique_ptr<cppLox::Types::Object>> m_objects;
    /// @brief The set of all strings that are currently allocated.
    std::unordered_set<cppLox::Types::ObjectString *, std::hash<cppLox::Types::ObjectString>,
                       cppLox::Types::SimpleComperator<cppLox::Types::ObjectString>>
        m_strings;
};