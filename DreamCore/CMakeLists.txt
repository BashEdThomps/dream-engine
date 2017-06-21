cmake_minimum_required (VERSION 2.8.0)

project(DreamCore)

file(GLOB_RECURSE SRC_DIR "*.cpp")
file(GLOB_RECURSE INC_DIR "*.h")

find_package(assimp REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(ALUT REQUIRED)

find_package(Lua          REQUIRED   )
message("Using Lua: "                )
message("\t L @ " ${LUA_LIBRARIES}   )
message("\t I @ " ${LUA_INCLUDE_DIR} )

message("Using LuaBind "                 )
message("\t I @ " ${Luabind_INCLUDE_DIRS} )

find_package(FREETYPE                       REQUIRED )
message("Using Freetype " ${FREETYPE_VERSION_STRING} )
message("\t L @ " ${FREETYPE_LIBRARIES}              )
message("\t I @ " ${FREETYPE_INCLUDE_DIR_ft2build}   )

include_directories(
    ${OPENAL_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}
    ${ALUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${assimp_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${Luabind_INCLUDE_DIRS}
    ${SOIL_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIR_ft2build}
)

add_library(DreamCore SHARED ${SRC_DIR})

target_link_libraries(
    DreamCore
    -lpthread
    -ldl
    Luabind
    SOIL
    ${LUA_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${OGGVORBIS_LIBRARIES}
    ${ALUT_LIBRARY}
    ${assimp_LIBRARIES}
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
