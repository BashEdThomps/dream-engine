cmake_minimum_required (VERSION 2.8.0)

project(DreamCore)

file(GLOB_RECURSE SRC_DIR "*.cpp")
file(GLOB_RECURSE INC_DIR "*.h")

set(OggVorbis_FIND_QUIETLY FALSE)

find_package(assimp REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(ALUT REQUIRED)
find_package(spdlog REQUIRED)
find_package(Lua REQUIRED)
find_package(Gainput REQUIRED)
find_package(TinySpline REQUIRED)
find_package(SOIL REQUIRED)
find_package(FFTW3 REQUIRED)
find_package(NanoVG REQUIRED)

message("Using Lua: "                )
message("\t L @ " ${LUA_LIBRARIES}   )
message("\t I @ " ${LUA_INCLUDE_DIR} )
message("\t V: "  ${LUA_VERSION_STRING})

find_package(FREETYPE                       REQUIRED )
message("Using Freetype " ${FREETYPE_VERSION_STRING} )
message("\t L @ " ${FREETYPE_LIBRARIES}              )
message("\t I @ " ${FREETYPE_INCLUDE_DIR_ft2build}   )

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
    ${FREETYPE_INCLUDE_DIR_ft2build}
    ${GAINPUT_INCLUDE_DIR}
    ${TINY_SPLINE_INCLUDE_DIR}
    ${SOIL_INCLUDE_DIR}
    ${FFTW3_INCLUDE_DIRS}
)

add_library(DreamCore SHARED ${SRC_DIR})

target_link_libraries(
    DreamCore
    -lpthread
    -ldl
    "-framework CoreFoundation"
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
    ${FREETYPE_LIBRARIES}
    ${FFTW3_LIBRARIES}
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
