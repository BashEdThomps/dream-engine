cmake_minimum_required (VERSION 2.8.0)

project (Dream)

set(DREAM_BUILD_SOIL    ON)
set(DREAM_BUILD_NANOVG  ON)
set(DREAM_BUILD_CORE    ON)
set(DREAM_BUILD_SDL     ON)

set(
  CMAKE_MODULE_PATH
  "${Dream_SOURCE_DIR}/CMakeModules"
)

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
endif ()

if(UNIX)
    message("Building for a *nix OS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=4096 --std=c++17")
endif()

if (APPLE)
    message(status "Building for MacOS X")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    #set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_MACOSX_RPATH 1)
endif()

# SOIL Library
if (DREAM_BUILD_SOIL)
    add_subdirectory(SOIL)
endif()

# NanoVG Library
if (DREAM_BUILD_NANOVG)
    add_subdirectory (NanoVG)
endif()

# DreamCore Library
if (DREAM_BUILD_CORE)
    add_subdirectory (DreamCore)
endif()

# DreamSDL Executable
if (DREAM_BUILD_SDL)
    add_subdirectory (DreamSDL)
endif()


