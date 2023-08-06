set(HEADER_FILES 
chunk.hpp
opcode.hpp
value.hpp
vm.hpp
)
set(SOURCE_FILES 
chunk.cpp
main.cpp
value.cpp
vm.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})