cmake_minimum_required(VERSION 3.24)

set(INTERPRETER_TESTS ${PROJECT_NAME}-tests)

#Fetches google-test framework from the github repo
include(AddDependency)
add_github_dependency(googletest google/googletest v1.14.0)

set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

add_compile_definitions(
  # Flag that indicates that test are executed
  CELLOX_TESTS_RUNNING
  # Directory of the cellox e2e tests
  TEST_PROGRAM_BASE_PATH="${PROJECT_SOURCE_DIR}/test/"
)

# Builds tests
add_executable(${INTERPRETER_TESTS} ${TEST_SOURCES} ${INTERPRETER_SOURCES})

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

#Links googletest and googlemock to the test executable
target_link_libraries(${INTERPRETER_TESTS} GTest::gtest_main GTest::gmock_main)

include(GoogleTest)

gtest_discover_tests(${INTERPRETER_TESTS})