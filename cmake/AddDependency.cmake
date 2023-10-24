include(FetchContent)
# Fetches a github repo and makes it available as a target
macro(add_github_dependency name url tag)
    if (NOT "${ARGN}" STREQUAL "")
        message(SEND_ERROR "add_github_dependency takes only a three argument.  Offending args: ${ARGN}")
    endif()    
set(full_url "https://github.com/${url}.git") 
    FetchContent_Declare(
        ${name}
        GIT_REPOSITORY ${full_url}
        GIT_TAG ${tag}
    )
    FetchContent_MakeAvailable(${name})
endmacro()