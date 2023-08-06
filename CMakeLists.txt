cmake_minimum_required(VERSION 3.16)
project(cpplox VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(CLX_DEBUG_PRINT_BYTECODE "Determines whether the chunks are dissassembled and the bytecode is printed" OFF)
option(CLX_DEBUG_TRACE_EXECUTION "Determines whether the execution shall be traced" OFF)
option(CLX_DEBUG_STRESS_GARBAGE_COLLECTOR "Determines whether the garbage collector shall be stressed" OFF)
option(CLX_DEBUG_LOG_GARBAGE_COLLECTION "Determines whether the garbage collection be logged" OFF)

add_subdirectory(src)
add_subdirectory(test)