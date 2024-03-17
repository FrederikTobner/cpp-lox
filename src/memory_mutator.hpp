/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file memory_mutator.hpp
 * @brief This file contains the memory mutator that manages the creation and deletion of all objects.
 */

#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types/object.hpp"
#include "types/object_native_fuction.hpp"
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

    template <cppLox::Types::DerivedFromObject T> auto registerObject(T * object) -> cppLox::Types::Object * {
        auto objWidened = static_cast<cppLox::Types::Object *>(object);
        m_objects.push_back(std::unique_ptr<cppLox::Types::Object>(objWidened));
        return objWidened;
    }

    /// @brief Sets the global variable with the given name to the given value.
    /// @param key The name of the global variable to set.
    /// @param value The value to set the global variable to.
    /// @return True if the variable already existed, false if it was created.
    auto setGlobal(cppLox::Types::ObjectString * key, cppLox::Types::Value value) -> bool {
        auto bucket = m_globals.find(key);
        bool exists = bucket != m_globals.end();
        if (exists) {
            bucket->second = value;
        } else {
            m_globals[key] = value;
        }
        return exists;
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
        return m_globals.erase(key);
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