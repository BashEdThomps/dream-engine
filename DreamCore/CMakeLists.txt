cmake_minimum_required (VERSION 2.8.0)

project(DreamCore)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cpp" "*.h" "*.hpp")

find_package(ALUT REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLM REQUIRED)
find_package(Lua REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SPDLOG REQUIRED)

include_directories(
    ${ALUT_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${OGG_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}
)

if (WIN32)
    add_library(
        DreamCore
        ${SRC_FILES}
    )
else()
    add_library(
        DreamCore
        SHARED
        ${SRC_FILES}
    )
endif()

IF (WIN32)
        target_link_libraries(
            DreamCore
            ${ALUT_LIBRARY}
            ${ASSIMP_LIBRARY_RELEASE}
            ${BULLET_LIBRARY}
            ${GLEW_LIBRARY_RELEASE}
            ${LUA_LIBRARIES}
            ${OGGVORBIS_LIBRARIES}
            ${OPENAL_LIBRARY}
            ${OPENGL_LIBRARIES}
        )
ELSE()
        target_link_libraries(
            DreamCore
            -lpthread
            -ldl
            "-framework CoreFoundation"
            ${ALUT_LIBRARY}
            ${ASSIMP_LIBRARIES}
            ${BULLET_LIBRARY}
            ${GLEW_LIBRARY_RELEASE}
            ${LUA_LIBRARIES}
            ${OGGVORBIS_LIBRARIES}
            ${OPENAL_LIBRARY}
            ${OPENGL_LIBRARIES}
        )
ENDIF()

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
