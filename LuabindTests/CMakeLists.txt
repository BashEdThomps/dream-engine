CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

# Define useful variables
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 9)
SET(VERSION_PATCH 2)

PROJECT("LuabindTests" CXX C)

# Annoying, but should leave this on really
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

find_package(Lua REQUIRED            )
message("Using Lua: "                )
message("\t L @ " ${LUA_LIBRARIES}   )
message("\t I @ " ${LUA_INCLUDE_DIR} )

FIND_PACKAGE(Boost REQUIRED)

FILE(GLOB SRC *.cpp *.h)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LUA_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Luabind_INCLUDE_DIRS}
)

add_executable(
    "LuabindTests"
    ${SRC}
)

IF(UNIX)
  TARGET_LINK_LIBRARIES("LuabindTests" -lc)
ENDIF()


TARGET_LINK_LIBRARIES(
    "LuabindTests"
    Luabind
    ${LUA_LIBRARIES}
)
