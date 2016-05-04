cmake_minimum_required (VERSION 2.8.0)
project (dream)

if(UNIX)
  message("CMAKE: Building for Unix")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif(UNIX)

if (APPLE)
  message("CMAKE: Building for MacOS X")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_MACOSX_RPATH 1)
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)
endif (APPLE)

set(CMAKE_BUILD_TYPE Debug)

# Find Required Libraries

find_library(OPENAL OpenAl)
if (NOT OPENAL)
  message(FATAL_ERROR "OpenAL not found")
else()
  message("Found OpenAL: " ${OPENAL})
endif()

find_library(V8 v8)
if (NOT V8)
  message(FATAL_ERROR "v8 not found")
else()
  message("Found v8: " ${V8})
endif()

find_library(V8_BASE v8_base)
if (NOT V8_BASE)
  message(FATAL_ERROR "v8_base not found")
else()
  message("Found v8_base: " ${V8_BASE})
endif()

find_library(V8_EXT_SNAP v8_external_snapshot)
if (NOT V8_EXT_SNAP)
  message(FATAL_ERROR "v8_external_snapshot not found")
else()
  message("Found v8_external_snapshot: " ${V8_EXT_SNAP})
endif()

find_library(V8_LIBBASE v8_libbase)
if (NOT V8_LIBBASE)
  message(FATAL_ERROR "v8_libbase not found")
else()
  message("Found v8_libbase: " ${V8_LIBBASE})
endif()

find_library(V8_LIBPLATFORM v8_libplatform)
if (NOT V8_LIBPLATFORM)
  message(FATAL_ERROR "v8_libplatform not found")
else()
  message("Found v8_libplatform: " ${V8_LIBPLATFORM})
endif()

find_library(V8_NOSNAPSHOT v8_nosnapshot)
if (NOT V8_NOSNAPSHOT)
  message(FATAL_ERROR "v8_nosnapshot not found")
else()
  message("Found v8_nosnapshot: " ${V8_NOSNAPSHOT})
endif()

find_library(PTHREAD pthread)
if (NOT PTHREAD)
  message(FATAL_ERROR "pthread not found")
else()
  message("Found pthread: " ${PTHREAD})
endif()

find_library(DL dl)
if (NOT DL)
  message(FATAL_ERROR "dl not found")
else()
  message("Found dl: " ${DL})
endif()

find_library(BULLET_DYNAMICS BulletDynamics)
if (NOT BULLET_DYNAMICS)
  message(FATAL_ERROR "BulletDynamics not found")
else()
  message("Found BulletDynamics: " ${BULLET_DYNAMICS})
endif()

find_library(BULLET_COLLISION BulletCollision)
if (NOT BULLET_COLLISION)
  message(FATAL_ERROR "BulletCollision not found")
else()
  message("Found BulletCollision: " ${BULLET_COLLISION})
endif()

find_library(LINEAR_MATH LinearMath)
if (NOT LINEAR_MATH)
  message(FATAL_ERROR "LinearMath not found")
else()
  message("Found LinearMath: " ${LINEAR_MATH})
endif()

# Dream
add_subdirectory (src)

# Dream Unit Tests
add_subdirectory (test)

# Enable Unit Testing
enable_testing()
add_test(NAME DreamTest COMMAND test/DreamTest)
