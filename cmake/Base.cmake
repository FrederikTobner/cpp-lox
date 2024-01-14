# Creates an option with the given name, description, and default value and prints its value during the configuration sof cmaketep.
macro(clx_option name description default_value)
    if (NOT "${ARGN}" STREQUAL "")
        message(SEND_ERROR "clx_option takes only a three argument.  Offending args: ${ARGN}")
    endif()    
    option(${name} ${description} ${default_value})
    message(STATUS "${name} ${${name}}")
endmacro()
