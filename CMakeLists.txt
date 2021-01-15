cmake_minimum_required (VERSION 3.0)

project (Dream C CXX)

set (CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("****************************************")
    message("             DEBUB Build!               ")
    message("****************************************")
    set(DREAM_LOG ON)
    set(CMAKE_VERBOSE_MAKEFILE OFF)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("****************************************")
    message("             Release Build!             ")
    message("****************************************")
    set(DREAM_LOG OFF)
    set(CMAKE_VERBOSE_MAKEFILE OFF)
endif()

set (CMAKE_SKIP_RULE_DEPENDENCY TRUE)
set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fvisibility=default ")

set(DREAM_BUILD_CORE ON)
set(DREAM_BUILD_GLFW ON)
set(DREAM_BUILD_TOOL ON)
set(DREAM_BUILD_DOC  ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

if (DREAM_LOG)
    if(WIN32)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DDREAM_LOG")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDREAM_LOG")
    else()
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDREAM_LOG")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDREAM_LOG")
    endif()
endif()

# Include Dependencies List
include(Dependencies/CMakeDependencies.txt)

# DreamCore Library
if (DREAM_BUILD_CORE)
    add_subdirectory (DreamCore)
endif()

# DreamGLFW Executable
if (DREAM_BUILD_GLFW)
    add_subdirectory (DreamGLFW)
endif()

# DreamTool Executable
if (DREAM_BUILD_TOOL)
    add_subdirectory (DreamTool)
endif()

# Doxygen Docs
if (DREAM_BUILD_DOC)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")
        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
      message(FATAL_ERROR "Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()
