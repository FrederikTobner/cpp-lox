set(HEADER_FILES 
chunk.hpp
lexer.hpp
opcode.hpp
token.hpp
value.hpp
vm.hpp
)
set(SOURCE_FILES 
chunk.cpp
lexer.cpp
main.cpp
token.cpp
value.cpp
vm.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})