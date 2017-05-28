CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

# Define useful variables
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 9)
SET(VERSION_PATCH 2)

PROJECT("Luabind" CXX C)

find_package(Lua REQUIRED            )
message("Using Lua: "                )
message("\t L @ " ${LUA_LIBRARIES}   )
message("\t I @ " ${LUA_INCLUDE_DIR} )

FIND_PACKAGE(Boost REQUIRED)

FILE(GLOB SRC src/*.cpp src/*.h)
FILE(GLOB HEADERS luabind/*.hpp)
FILE(GLOB HEADERS_DETAIL luabind/detail/*.hpp)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LUA_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIRS}
)

add_library(
    Luabind
    SHARED
    ${SRC}
    ${HEADERS}
    ${HEADERS_DETAIL}
)

IF(UNIX)
  TARGET_LINK_LIBRARIES("Luabind" -lc)
ENDIF()


TARGET_LINK_LIBRARIES(
    "Luabind"
    ${LUA_LIBRARIES}
)

message("Luabind project name: " ${PROJECT_NAME})
message("Luabind src dir: " ${PROJECT_SOURCE_DIR})

set(
    ${PROJECT_NAME}_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE
)

set(
    ${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
message("Luabind Exporting...")
message("\t I @ " ${Luabind_INCLUDE_DIRS} )
