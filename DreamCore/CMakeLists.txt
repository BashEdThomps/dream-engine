cmake_minimum_required (VERSION 2.8.0)

project(DreamCore)

file(GLOB_RECURSE SRC_DIR "*.cpp")
file(GLOB_RECURSE INC_DIR "*.h")

find_package(BULLET   REQUIRED)
find_package(OPENGL   REQUIRED)
find_package(GLM      REQUIRED)
find_package(GLEW     REQUIRED)
find_package(assimp   REQUIRED)
find_package(SOIL     REQUIRED)
find_package(Lua      REQUIRED)
find_package(luabind  REQUIRED)
find_package(FREETYPE REQUIRED)

message("Using Lua     @ L:" ${LUA_LIBRARIES} " I:" ${LUA_INCLUDE_DIR})
message("Using LuaBind @ L:" ${LUABIND_LIBRARIES} " I:" ${LUABIND_INCLUDE_DIR})

message("Using Freetype " ${FREETYPE_VERSION_STRING})
message("\t @ " ${FREETYPE_INCLUDE_DIR_ft2build})
message("\t @ " ${FREETYPE_LIBRARIES})

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${assimp_INCLUDE_DIRS}
    ${SOIL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR_ft2build}
)

add_library(DreamCore SHARED ${SRC_DIR})

target_link_libraries(
    DreamCore
    -lpthread
    -ldl
    ${LUA_LIBRARIES}
    ${LUABIND_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${assimp_LIBRARIES}
    ${SOIL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${BULLET_LIBRARY}
    ${FREETYPE_LIBRARIES}
)

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
