# Build the Dream Library and Executable.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Main Files
file(GLOB_RECURSE DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main/Main.cpp")

# Targets
add_library (DreamBase SHARED ${DREAM_SRC})

# Source Groups

# Find Required Packages
find_package(BULLET REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM    REQUIRED)
find_package(GLEW   REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL   REQUIRED)
find_package(Chaiscript REQUIRED)

# Libraries

target_link_libraries(
  DreamBase
  -lpthread
  -ldl
  ${OPENGL_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${SDL2_LIBRARY}
  ${SDLMIXER_LIBRARY}
  ${GLEW_LIBRARIES}
  ${BULLET_LIBRARY}
)

include_directories(
  ${CHAISCRIPT_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${SDLMIXER_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
)

add_executable (Dream "Main.cpp")
source_group(Main FILES "Main.cpp")

target_link_libraries(
  Dream
  DreamBase
  -lpthread
  -ldl
  ${SDL2_LIBRARIES}
  ${SDLMIXER_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARY}
)
