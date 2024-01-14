#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types/object.hpp"
#include "types/object_string.hpp"
#include "types/value.hpp"

#include "simple_comperator.hpp"
#include "string_hash.hpp"

namespace cppLox {

/// @brief The memory mutator that manages the creation and deletion of all objects.
class MemoryMutator {

  public:
    /// @brief Creates a new memory mutator.
    MemoryMutator() = default;

    /// @brief Destroys the memory mutator.
    ~MemoryMutator() = default;

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
            auto iterator = this->m_strings.find(string);
            if (iterator != m_strings.end()) {
                delete string;
                return static_cast<cppLox::Types::Object *>(*iterator);
            }
            m_strings.insert(string);
            object = static_cast<cppLox::Types::Object *>(string);
        } else {
            object = static_cast<cppLox::Types::Object *>(new T(std::forward<Args>(args)...));
        }
        m_objects.push_back(std::unique_ptr<cppLox::Types::Object>(object));
        return object;
    }

    /// @brief Sets the global variable with the given name to the given value.
    /// @param key The name of the global variable to set.
    /// @param value The value to set the global variable to.
    /// @return True if the variable already existed, false if it was created.
    auto setGlobal(cppLox::Types::ObjectString * key, cppLox::Types::Value value) -> bool {
        auto valueBucket = m_globals.find(key);
        m_globals[key] = value;
        return valueBucket != m_globals.end();
    }

    /// @brief Gets the global variable with the given name.
    /// @param key The name of the global variable to get.
    /// @return The value of the global variable.
    auto getGlobal(cppLox::Types::ObjectString * key) const -> cppLox::Types::Value {
        auto iterator = m_globals.find(key);
        if (iterator != m_globals.end()) {
            return iterator->second;
        }
        return cppLox::Types::Value();
    }

    /// @brief Deletes the global variable with the given name.
    /// @param key The name of the global variable to delete.
    /// @return True if the variable was deleted, false if it didn't exist.
    auto deleteGlobal(cppLox::Types::ObjectString * key) -> bool {
        auto iterator = m_globals.find(key);
        if (iterator != m_globals.end()) {
            m_globals.erase(iterator);
            return true;
        }
        return false;
    }

  private:
    /// @brief The list of all objects that are currently allocated.
    std::vector<std::unique_ptr<cppLox::Types::Object>> m_objects;

    /// @brief The set of all strings that are currently allocated.
    std::unordered_set<cppLox::Types::ObjectString *, std::hash<cppLox::Types::ObjectString>,
                       cppLox::Types::SimpleComperator<cppLox::Types::ObjectString>>
        m_strings;

    /// @brief The map of all global variables.
    std::unordered_map<cppLox::Types::ObjectString *, cppLox::Types::Value, std::hash<cppLox::Types::ObjectString>,
                       cppLox::Types::SimpleComperator<cppLox::Types::ObjectString>>
        m_globals;
};
} // namespace cppLox