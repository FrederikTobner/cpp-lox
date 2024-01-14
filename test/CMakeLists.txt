cmake_minimum_required(VERSION 3.24)

set(INTERPRETER_TESTS ${PROJECT_NAME}-tests)

#Fetches google-test framework from the github repo
include(AddDependency)
add_github_dependency(googletest google/googletest v1.14.0)

set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

# Builds tests
add_executable(${INTERPRETER_TESTS} ${TEST_SOURCES} ${INTERPRETER_SOURCES})

# Includes SourcePath of the compiler for shorter includes and the config file
target_include_directories(${INTERPRETER_TESTS} PUBLIC ${SOURCEPATH} ${PROJECT_BINARY_DIR}/src)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  add_compile_definitions(BUILD_TYPE_DEBUG)
endif()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#Links googletest and googlemock to the test executable
target_link_libraries(${INTERPRETER_TESTS} GTest::gtest_main GTest::gmock_main)

include(GoogleTest)

gtest_discover_tests(${INTERPRETER_TESTS})