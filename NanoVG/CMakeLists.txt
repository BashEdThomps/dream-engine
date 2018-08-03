cmake_minimum_required (VERSION 2.8.0)
project(NanoVG C)

set(
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${PROJECT_SOURCE_DIR}/CMakeModules"
)

IF(APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKIT_LIBRARY IOKit)
    FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
ENDIF()

find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OPENGL REQUIRED)

file(
    GLOB_RECURSE
    NANOVG_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

file(GLOB NANOVG_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/nanovg.h")


# libNanoGV static library

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

add_library(
    NanoVG
    SHARED
    ${NANOVG_SOURCES}
)

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src/nanovg.h
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
message("${PROJECT_NAME} Exporting...")
message("\t I @ " ${NANOVG_INCLUDE_DIRS} )
