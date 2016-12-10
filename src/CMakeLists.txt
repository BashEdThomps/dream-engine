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
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM    REQUIRED)
find_package(GLEW   REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL   REQUIRED)
find_package(Lua REQUIRED)
find_package(luabind REQUIRED)

# Libraries

target_link_libraries(
  DreamBase
  -lpthread
  -ldl
  ${OPENGL_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${BULLET_LIBRARY}

  ${LUA_LIBRARIES}
  ${LUABIND_LIBRARIES}

  ${SDL2_LIBRARY}
  ${SDLMIXER_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY}
)

message("Using lua L: " ${LUA_LIBRARIES} " H: " ${LUA_INCLUDE_DIR})
message("Using LuaBind L: " ${LUABIND_LIBRARIES} " H: " ${LUABIND_INCLUDE_DIR})

include_directories(
  ${CHAISCRIPT_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}

  ${LUA_INCLUDE_DIR}
  ${LUABIND_INCLUDE_DIR}

  ${SDL2_INCLUDE_DIR}
  ${SDLMIXER_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
)

add_executable (Dream "Main.cpp")
source_group(Main FILES "Main.cpp")

target_link_libraries(
  Dream
  DreamBase
  -lpthread
  -ldl

  ${GLEW_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARY}

  ${LUA_LIBRARIES}
  ${LUABIND_LIBRARIES}

  ${SDL2_LIBRARY}
  ${SDLMIXER_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY}
)
