cmake_minimum_required (VERSION 2.8.0)

project (Dream)

set(DREAM_BUILD_LUABIND ON)
set(DREAM_BUILD_CORE ON)
set(DREAM_BUILD_SDL ON)

set(DREAM_BUILD_LUABIND_TESTS OFF) # Broken, possible boost version issues :'(
set(DREAM_BUILD_TESTS OFF)         # Needs updating, badly...

set(
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${Dream_SOURCE_DIR}/CMakeModules"
)

message("Using modules path: " ${CMAKE_MODULE_PATH})

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)



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
if (DREAM_BUILD_LUABIND)
    add_subdirectory(Luabind)
endif()

# Luabind Library
if (DREAM_BUILD_LUABIND_TESTS)
    add_subdirectory(LuabindTests)
endif()


# DreamCore Library
if (DREAM_BUILD_CORE)
    add_subdirectory (DreamCore)
endif()

# Core Tests
if (DREAM_BUILD_TESTS)
    add_subdirectory (DreamCoreTests)
endif()

# DreamSDL Executable
if (DREAM_BUILD_SDL)
    add_subdirectory (DreamSDL)
endif()
