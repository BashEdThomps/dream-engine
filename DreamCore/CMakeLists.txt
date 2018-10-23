cmake_minimum_required (VERSION 2.8.0)

project(DreamCore CXX)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cpp")
file(GLOB_RECURSE INC_FILES "*.h" "*.hpp")


find_package(assimp REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(ALUT REQUIRED)
find_package(Lua REQUIRED)


message("Using Lua: "                )
message("\t L @ " ${LUA_LIBRARIES}   )
message("\t I @ " ${LUA_INCLUDE_DIR} )
message("\t V: "  ${LUA_VERSION_STRING})

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}
    ${ALUT_INCLUDE_DIR}
    ${assimp_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
)

add_library(
    DreamCore
    SHARED
    ${SRC_FILES}
)

target_link_libraries(
    DreamCore
    -lpthread
    -ldl
    "-framework CoreFoundation"
    "-framework IOKit"
    ${LUA_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${OGGVORBIS_LIBRARIES}
    ${ALUT_LIBRARY}
    ${assimp_LIBRARIES}
    ${BULLET_LIBRARY}
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
