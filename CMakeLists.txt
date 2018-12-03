cmake_minimum_required (VERSION 2.8.0)

project (Dream C CXX)

set(DREAM_BUILD_CORE ON)
set(DREAM_BUILD_GLFW OFF)
set(DREAM_BUILD_TOOL ON)

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
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(RELAX_CLANG ON)

if (APPLE)
    message(status "Building for MacOS X")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=4096 --std=c++14")
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
