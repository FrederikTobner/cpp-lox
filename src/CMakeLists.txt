# configure cellox config to show version
configure_file(cpplox-config.hpp.in cpplox-config.hpp)

add_executable(${PROJECT_NAME} ${INTERPRETER_SOURCES})

# for including the cpplox-config.hpp file
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/src)