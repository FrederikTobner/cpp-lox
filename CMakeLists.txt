cmake_minimum_required(VERSION 3.16)
project(cpplox VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake Macros
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Base)

# CMake Options
clx_option(CLX_DEBUG_PRINT_BYTECODE "Determines whether the chunks are dissassembled and the bytecode is printed" OFF)
clx_option(CLX_DEBUG_TRACE_EXECUTION "Determines whether the execution shall be traced" OFF)
clx_option(CLX_DEBUG_STRESS_GARBAGE_COLLECTOR "Determines whether the garbage collector shall be stressed" OFF)
clx_option(CLX_DEBUG_LOG_GARBAGE_COLLECTION "Determines whether the garbage collection be logged" OFF)
clx_option(CLX_BUILD_TESTS "Determines whether the tests shall be built" OFF)

# Looks up all the files in the src directory that end with .cpp (Recursively)
file(GLOB_RECURSE INTERPRETER_SOURCES src/*.cpp)

add_subdirectory(src)

if(CLX_BUILD_TESTS)
    enable_testing()
    # Remove main.cpp from the sources, as it is not needed for the tests
    list(REMOVE_ITEM INTERPRETER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_subdirectory(test)
endif()