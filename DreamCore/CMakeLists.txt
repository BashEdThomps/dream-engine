cmake_minimum_required (VERSION 2.8.0)

project(DreamCore CXX)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cpp")
file(GLOB_RECURSE INC_FILES "*.h" "*.hpp")


find_package(BULLET REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(ALUT REQUIRED)
find_package(Lua REQUIRED)
find_package(GLM REQUIRED)


if (WIN32)
    find_package(GLEW REQUIRED)
    include_directories(
            ${OPENGL_INCLUDE_DIR}
            ${OPENAL_INCLUDE_DIR}
            ${VORBIS_INCLUDE_DIR}
            ${ALUT_INCLUDE_DIR}
            ${assimp_INCLUDE_DIRS}
            ${GLM_INCLUDE_DIR}
            ${BULLET_INCLUDE_DIR}
            ${LUA_INCLUDE_DIR}
            ${SOIL_INCLUDE_DIRS}
            ${NANOVG_INCLUDE_DIRS}
            ${SPDLOG_INCLUDE_DIR}
            ${GAINPUT_INCLUDE_DIR}
            ${TINY_SPLINE_INCLUDE_DIR}
            ${SOIL_INCLUDE_DIR}
            ${FFTW3_INCLUDE_DIRS}
            ${GLEW_INCLUDE_DIR}
        )
else()
    include_directories(
            ${OPENGL_INCLUDE_DIR}
            ${OPENAL_INCLUDE_DIR}
            ${VORBIS_INCLUDE_DIR}
            ${ALUT_INCLUDE_DIR}
            ${assimp_INCLUDE_DIRS}
            ${GLM_INCLUDE_DIR}
            ${BULLET_INCLUDE_DIR}
            ${LUA_INCLUDE_DIR}
            ${SOIL_INCLUDE_DIRS}
            ${NANOVG_INCLUDE_DIRS}
            ${SPDLOG_INCLUDE_DIR}
            ${GAINPUT_INCLUDE_DIR}
            ${TINY_SPLINE_INCLUDE_DIR}
            ${SOIL_INCLUDE_DIR}
            ${FFTW3_INCLUDE_DIRS}
        )
endif()

<<<<<<< HEAD
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}
    ${ALUT_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)
=======

>>>>>>> windows_port

add_library(
    DreamCore
    SHARED
    ${SRC_FILES}
)

<<<<<<< HEAD
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
    ${BULLET_LIBRARY}
    ${GLM_LIBRARY}
)
=======
IF (WIN32)
        target_link_libraries(
            DreamCore
            ${GLEW_LIBRARY_RELEASE}
            ${SOIL_LIB}
            ${TINY_SPLINE_LIB}
            ${TINY_SPLINE_CPP_LIB}
            ${GAINPUT_LIB}
            ${LUA_LIBRARIES}
            ${OPENAL_LIBRARY}
            ${OPENGL_LIBRARIES}
            ${OGGVORBIS_LIBRARIES}
            ${NANOVG_LIB}
            ${ALUT_LIBRARY}
            ${assimp_LIBRARIES}
            ${BULLET_LIBRARY}
            ${FFTW3_LIBRARIES}
        )
ELSE()
        target_link_libraries(
            DreamCore
            -lpthread
            -ldl
            "-framework CoreFoundation"
            -lGLEW
            ${SOIL_LIB}
            ${TINY_SPLINE_LIB}
            ${TINY_SPLINE_CPP_LIB}
            ${GAINPUT_LIB}
            ${LUA_LIBRARIES}
            ${OPENAL_LIBRARY}
            ${OPENGL_LIBRARIES}
            ${OGGVORBIS_LIBRARIES}
            ${NANOVG_LIB}
            ${ALUT_LIBRARY}
            ${assimp_LIBRARIES}
            ${BULLET_LIBRARY}
            ${FFTW3_LIBRARIES}
        )
ENDIF()

>>>>>>> windows_port

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
