# Creates a library target with the provided 'name' from the given source files.
macro(add_cpplox_library name)
  if(BUILD_SHARED_LIBS)
    set(LIBTYPE SHARED)
  else()
    set(LIBTYPE STATIC)
  endif()
  add_library(${name} ${LIBTYPE} ${ARGN})
  if(NOT CMAKE_BUILD_TYPE MATCHES  "[Dd][Ee][Bb][Uu][Gg]")  
  install(TARGETS ${name}
      COMPONENT ${name}
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
      RUNTIME DESTINATION bin)
   endif()
endmacro()