# Creates an executable target with the provided 'name' from the given source files.
macro(add_cpplox_executable name)
  add_executable(${name} ${ARGN} )
  if(NOT CMAKE_BUILD_TYPE MATCHES  "[Dd][Ee][Bb][Uu][Gg]")
  install(TARGETS ${name}
        COMPONENT ${name}
        RUNTIME DESTINATION bin)
   endif()
endmacro()