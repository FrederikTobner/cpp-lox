#pragma once

#include "memory_mutator.hpp"
#include "object_string.hpp"

/// Concatenates the given strings.
/// @param left The left string.
/// @param right The right string.
/// @return The concatenated string.
auto concatenate_strings(ObjectString * left, ObjectString * right, MemoryMutator * memoryMutator) -> Object * {
    return memoryMutator->create<ObjectString>(left->string() + right->string());
}