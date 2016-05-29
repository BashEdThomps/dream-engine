# Build the Dream Library and Executable.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Main Files
file(GLOB_RECURSE DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main/Main.cpp")

# Targets
add_library (DreamBase SHARED ${DREAM_SRC})

# Source Groups

file(GLOB_RECURSE _ASSET_SRC "Asset/*.h" "Asset/*.cpp")
source_group(Asset FILES ${_ASSET_SRC})

file(GLOB_RECURSE _JSON_SRC "JSON/*.hpp")
source_group(Json FILES ${_JSON_SRC})

file(GLOB_RECURSE _MAIN_SRC "Main/*.h" "Main/*.cpp")
list(REMOVE_ITEM  _MAIN_SRC "Main/Main.cpp")
source_group(Main FILES ${_MAIN_SRC})

file(GLOB_RECURSE _PLUGINS_SRC "Plugins/*.h" "Plugins/*.cpp")
source_group(Plugins FILES ${_PLUGINS_SRC})

file(GLOB_RECURSE _PROJECT_SRC "Project/*.h" "Project/*.cpp")
source_group(Project FILES ${_PROJECT_SRC})

file(GLOB_RECURSE _SCENE_SRC "Scene/*.h" "Scene/*.cpp")
source_group(Scene FILES ${_SCENE_SRC})

file(GLOB_RECURSE _UNIT_SRC "Unit/*.h" "Unit/*.cpp")
source_group(Unit FILES ${_UNIT_SRC})

file(GLOB_RECURSE _UTIL_SRC "Util/*.h" "Util/*.cpp")
source_group(Util FILES ${_UTIL_SRC})


# Find Required Packages
find_package(OPENAL REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLFW   REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM    REQUIRED)
find_package(GLEW   REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL   REQUIRED)
find_package(Lua52  REQUIRED)

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
  ${LUA_LIBRARIES}
)

include_directories(
  ${OPENAL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${assimp_INCLUDE_DIRS}
	${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
)

add_executable (Dream "Main/Main.cpp")
source_group(Main FILES "Main/Main.cpp")

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
  ${LUA_LIBRARIES}
)
