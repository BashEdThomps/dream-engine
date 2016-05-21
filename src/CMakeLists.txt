# Build the Dream Library and Executable.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Main Files
file(GLOB_RECURSE DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main/Main.cpp")

# Find Required Libraries
find_package(OPENAL COMPONENTS OpenAL REQUIRED)
if (NOT OPENAL_FOUND)
  message(FATAL_ERROR "Package OpenAL not found")
endif()

find_package(BULLET COMPONENTS Bullet REQUIRED)
if (NOT BULLET_FOUND)
  message(FATAL_ERROR "Package Bullet not found")
endif()

find_package(V8 COMPONENTS V8 REQUIRED)
if (NOT V8_FOUND)
  message(FATAL_ERROR "Package v8 not found")
endif()

find_package(GLFW COMPONENTS GLFW REQUIRED)
if (NOT GLFW_FOUND)
  message(FATAL_ERROR "Package GLFW Not Found")
endif()

find_package(CHAISCRIPT COMPONENTS Chaiscript REQUIRED)
if (NOT CHAISCRIPT_FOUND)
  message(FATAL_ERROR "Package ChaiScript was not found")
endif()

find_package(OPENGL COMPONENTS OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message(FATAL_ERROR "Package OpenGL was not found")
else()
  message("OpenGL Headers found at " ${OPENGL_INCLUDE_DIR})
  message("OpenGL Lib found at " ${OPENGL_LIBRARIES})
endif()

# Targets
add_library (
    DreamBase SHARED
    ${DREAM_SRC}
)

target_link_libraries(
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)

include_directories(
  ${CHAISCRIPT_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${V8_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
)

add_executable (Dream Main/Main.cpp)

target_link_libraries(
  Dream
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)
