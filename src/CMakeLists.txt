# Build the Dream Library and Executable.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Main Files
file(GLOB_RECURSE DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main/Main.cpp")

# Targets
add_library (DreamBase SHARED ${DREAM_SRC})

# Source Groups

# Find Required Packages
find_package(OPENAL REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLFW   REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM    REQUIRED)
find_package(GLEW   REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL   REQUIRED)
find_package(Chaiscript REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(ALUT REQUIRED)

# Libraries

target_link_libraries(
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${BULLET_LIBRARY}
  ${OGGVORBIS_LIBRARIES}
  ${ALUT_LIBRARY}
)

include_directories(
  ${CHAISCRIPT_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${ALUT_INCLUDE_DIR}
)

add_executable (Dream "Main.cpp")
source_group(Main FILES "Main.cpp")

target_link_libraries(
  Dream
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARY}
  ${OGGVORBIS_LIBRARIES}
  ${ALUT_LIBRARY}
)
