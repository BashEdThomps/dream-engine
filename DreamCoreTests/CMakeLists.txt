cmake_minimum_required (VERSION 2.8.0)
project(DreamCoreTests CXX)

file(GLOB_RECURSE SRC "*.h" "*.hpp" "*.cpp")

find_package(BULLET REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL REQUIRED)
find_package(Lua REQUIRED)
find_package(FREETYPE REQUIRED)

include_directories(
  ${Luabind_INCLUDE_DIRS}
  ${DreamCore_INCLUDE_DIRS}

  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR_ft2build}
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}
)

target_link_libraries(
  ${PROJECT_NAME}

  Luabind
  DreamCore
  -lpthread
  -ldl

  ${GLEW_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARY}
  ${LUA_LIBRARIES}
  ${LUABIND_LIBRARIES}
  ${FREETYPE_LIBRARIES}
)
