cmake_minimum_required (VERSION 2.6)

project ("SOIL" C)

#set (SOIL_BUILD_TESTS false CACHE BOOL "Build tests")

find_package (OpenGL REQUIRED)

file (GLOB SRC      src/*.c)
file (GLOB HEADERS  include/*.h)
file (GLOB TEST_SRC test/*.c)

include_directories (${OPENGL_INCLUDE_DIR})

add_library (
    SOIL
    SHARED
    ${SRC}
    ${HEADERS}
)

if(APPLE)
    target_link_libraries (
        SOIL
       "-framework CoreFoundation"
        ${OPENGL_LIBRARIES}
    )
else()
    target_link_libraries (
        SOIL
        ${OPENGL_LIBRARIES}
    )
endif()

#[=[

install (
    TARGETS SOIL
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install (
    FILES
    ${SOIL_HEADERS} DESTINATION include/SOIL
)

if (SOIL_BUILD_TESTS)
    add_executable (test_SOIL src/test_SOIL.cpp)
    target_link_libraries (test_SOIL SOIL)
    install (TARGETS SOIL test_SOIL RUNTIME DESTINATION bin)
endif ()

#]=]

message("SOIL Project name: " ${PROJECT_NAME})
message("SOIL Src dir: " ${PROJECT_SOURCE_DIR})

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
message("SOIL Exporting...")
message("\t I @ " ${SOIL_INCLUDE_DIRS} )
