cmake_minimum_required (VERSION 2.8.0)
project(DreamSDL CXX)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(BULLET     REQUIRED)
find_package(SDL2       REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(GLM        REQUIRED)
find_package(GLEW       REQUIRED)
find_package(assimp     REQUIRED)
find_package(SOIL       REQUIRED)
find_package(Lua        REQUIRED)
find_package(luabind    REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
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
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}
)

target_link_libraries(
  ${PROJECT_NAME}
  DreamCore
  -lpthread
  -ldl
  ${GLEW_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARY}
  ${SDL2_LIBRARY}
  ${SDLMIXER_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${LUA_LIBRARIES}
  ${LUABIND_LIBRARIES}
)
