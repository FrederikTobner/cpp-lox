# configure cellox config to show version
configure_file(cpplox-config.hpp.in cpplox-config.hpp)

add_executable(${PROJECT_NAME} ${INTERPRETER_SOURCES})

# for including the cpplox-config.hpp file
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/src)

# Install configuration
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    if(CLX_DEBUG_PRINT_BYTECODE) 
        target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_PRINT_BYTECODE)
    endif()
    if(DEBUG_TRACE_EXECUTION)
        target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_TRACE_EXECUTION)
    endif()
    if(CLX_DEBUG_STRESS_GARBAGE_COLLECTOR)
        target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_STRESS_GARBAGE_COLLECTOR)
    endif()
    if(CLX_DEBUG_LOG_GARBAGE_COLLECTION)
        target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_LOG_GARBAGE_COLLECTION)
    endif()    
else()
    # CPack metadata
    set(PROJECT_VERSIONED_NAME "${PROJECT_NAME}_${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}") 
    set(PROJECT_PAGE "https://github.com/FrederikTobner/cpp-lox")
    set(PROJECT_HELP_PAGE "https://github.com/FrederikTobner/cpp-lox/wiki")
    set(PROJECT_VENDOR "Frederik Tobner")
    set(PROJECT_CONTACT "f.tobner@gmail.com")
    # Sets properties for the package created using cpack - https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
    # Setting source directory
    set(CPACK_BUILD_SOURCE_DIRS "${PROJECT_BINARY_DIR}/src")
    # Creates a desktop shortcut
    set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME})
    # Sets the package description
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} Interpreter")
    # Creates a short cut in the start menu
    set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} ${PROJECT_VERSIONED_NAME})
    # Configurating the installation directory
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_VERSIONED_NAME})
    # Setting the name of the package
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    # Setting the project vendor
    set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
    # Setting the project version
    set(CPACK_PACKAGE_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
    # Add license as resouce
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
    
    # Sets properties for NSIS (nullsoft scriptable install system - https://sourceforge.net/projects/nsis/)
    # Setting the branding text that is displayed at the bottom of the install window.   
    set(CPACK_NSIS_BRANDING_TEXT ${PROJECT_NAME})
    # Setting contact info
    set(CPACK_NSIS_CONTACT ${PROJECT_CONTACT})
    # Configurating help page
    set(CPACK_NSIS_HELP_LINK ${PROJECT_HELP_PAGE})
    # Add option to add the installed program to the path  
    set(CPACK_NSIS_MODIFY_PATH "ON")
    # Setting the icon if the installer
    set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/icons/file_type_cpplox.ico")
    # Setting the package name of the nsis package
    set(CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME})
    # About URL provided in the installer
    set(CPACK_NSIS_URL_INFO_ABOUT ${PROJECT_PAGE})    
    # Includes the CPack module 
    include(CPack)    
    # Configure install destination of the interpreter
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()