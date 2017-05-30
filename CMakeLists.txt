cmake_minimum_required (VERSION 2.8.0)

project (Dream)

set(
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${Dream_SOURCE_DIR}/CMakeModules"
)

message("Using modules path: " ${CMAKE_MODULE_PATH})

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_COLOR_MAKEFILE   ON)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

if(UNIX)
    message("Building for a *nix OS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

if (APPLE)
    message(status "Building for MacOS X")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_MACOSX_RPATH 1)
endif()

# Luabind Library
add_subdirectory(Luabind)

# DreamCore Library
add_subdirectory (DreamCore)
add_subdirectory (DreamCoreTests)

# DreamSDL Executable
add_subdirectory (DreamSDL)
