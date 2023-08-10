cmake_minimum_required(VERSION 3.24)

set(INTERPRETER_TESTS ${PROJECT_NAME}-tests)

#Fetches google-test framework from the github repo
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

# dependencies from the interpreter needed to build the tests
set(TEST_DEPENDENCIES_SOURCE_FILES
${SOURCE_PATH}/chunk.cpp
${SOURCE_PATH}/lexer.cpp
${SOURCE_PATH}/token.cpp
${SOURCE_PATH}/value.cpp
${SOURCE_PATH}/vm.cpp
)

set(TEST_DEPENDENCIES_HEADER_FILES
${SOURCE_PATH}/chunk.hpp
${SOURCE_PATH}/lexer.hpp
${SOURCE_PATH}/token.hpp
${SOURCE_PATH}/value.hpp
${SOURCE_PATH}/vm.hpp
)

add_compile_definitions(
# Flag that indicates that test are executed
CELLOX_TESTS_RUNNING
# Directory of the cellox e2e tests
TEST_PROGRAM_BASE_PATH="${PROJECT_SOURCE_DIR}/test/"
)

# Builds tests
add_executable(${INTERPRETER_TESTS} 
${TEST_SOURCES} ${INTERPRETER_SOURCES})

# Includes SourcePath of the compiler for shorter includes and the config file
target_include_directories(${INTERPRETER_TESTS} PUBLIC ${SOURCEPATH} ${PROJECT_BINARY_DIR}/src)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  # Debug options
  if(CLX_DEBUG_PRINT_BYTECODE)
    add_compile_definitions(DEBUG_PRINT_CODE)
  endif()
  if(CLX_DEBUG_TRACE_EXECUTION)
    add_compile_definitions(DEBUG_TRACE_EXECUTION)
  endif()
  if(CLX_DEBUG_STRESS_GARBAGE_COLLECTOR)
    add_compile_definitions(DEBUG_STRESS_GC)
  endif()
  if(CLX_DEBUG_LOG_GARBAGE_COLLECTOIN)
    add_compile_definitions(DEBUG_LOG_GC)
  endif()
  add_compile_definitions(BUILD_TYPE_DEBUG)
endif()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#Fetches google test
FetchContent_MakeAvailable(googletest)

#Links googletest and googlemock to the test executable
target_link_libraries(${INTERPRETER_TESTS} GTest::gtest_main GTest::gmock_main)

add_test(NAME ${INTERPRETER_TESTS} COMMAND ${INTERPRETER_TESTS})

include(GoogleTest)

gtest_discover_tests(${INTERPRETER_TESTS})