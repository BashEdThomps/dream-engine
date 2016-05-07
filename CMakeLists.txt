cmake_minimum_required (VERSION 2.8.0)
project (dream)

SET(CMAKE_MODULE_PATH CMakeModules)

if(UNIX)
  message("CMAKE: Building for Unix")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

if (APPLE)
  message("CMAKE: Building for MacOS X")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_MACOSX_RPATH 1)
endif()

# Build Debug
set(CMAKE_BUILD_TYPE Debug)

# DreamBase Library and Dream Executable
add_subdirectory (src)

# Dream Unit Tests
add_subdirectory (test)

# Enable Unit Testing
enable_testing()
add_test(NAME DreamTest COMMAND test/DreamTest)
