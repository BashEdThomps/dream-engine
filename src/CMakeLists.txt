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

find_package(GLM COMPONENTS glm REQUIRED)
if (NOT GLM_FOUND)
 message(FATAL_ERROR "GLM Package was Not Found")
else()
	message("Found GLM Headers at " ${GLM_INCLUDE_DIR})
endif()

find_package(GLEW COMPONENTS glew REQUIRED)
if (NOT GLEW_FOUND)
 message(FATAL_ERROR "GLEW Package was Not Found")
else()
	message("Found GLEW Headers at " ${GLEW_INCLUDE_DIRS})
	message("Found GLEW Libs at "    ${GLEW_LIBRARIES})
endif()

find_package(assimp COMPONENTS assimp REQUIRED)
if (NOT assimp_FOUND)
	message(FATAL_ERROR "Assimp Package Not Found")
else()
	message("Found AssImp Headers " ${assimp_INCLUDE_DIRS})
	message("Found AssImp Libs " ${assimp_LIBRARIES})
endif()

find_package(SOIL COMPONENTS SOIL REQUIRED)
if (NOT SOIL_FOUND)
	message(FATAL_ERROR "SOIL Package Not Found")
else()
	message("Found SOIL Headers "   ${SOIL_INCLUDE_DIR})
	message("Found SOIL Libraries " ${SOIL_LIBRARIES})
endif()


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
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
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
  ${V8_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
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
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)
