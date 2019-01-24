cmake_minimum_required (VERSION 2.8.0)

project (Dream C CXX)

set(DREAM_LOG ON)
set(DREAM_BUILD_CORE ON)
set(DREAM_BUILD_GLFW ON)
set(DREAM_BUILD_TOOL ON)
set(DREAM_BUILD_DOC  OFF)

set(CMAKE_MODULE_PATH "${Dream_SOURCE_DIR}/CMakeModules")
message("Using modules path: " ${CMAKE_MODULE_PATH})

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  # BigObj is required by sol2
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
  message("MSVC Version: ${MSVC_VERSION} | Toolset: ${MSVC_TOOLSET_VERSION}")
endif()

set(RELAX_CLANG ON)

if (APPLE)
    message(status "Building for MacOS X")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++1z")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    if (RELAX_CLANG)
        set(
            CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} \
            -Wno-incompatible-pointer-types-discards-qualifiers \
            -Wno-unused-result \
            -Wno-comma \
            -Wno-unused-variable"
        )
    endif()
    set(CMAKE_MACOSX_RPATH 1)
endif()

if(UNIX)
    message("Building for a *nix OS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=4096 --std=c++1z")
endif()

if (DREAM_LOG)
    if(WIN32)
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDREAM_LOG")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDREAM_LOG")
	else()
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DDREAM_LOG")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDREAM_LOG")
	endif()
endif()
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
      message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()
