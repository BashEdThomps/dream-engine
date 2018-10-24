cmake_minimum_required (VERSION 2.8.0)
project(DreamSDL CXX)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(BULLET     REQUIRED)
find_package(SDL2       REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(GLM        REQUIRED)
find_package(Lua        REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}
)

target_link_libraries(
    ${PROJECT_NAME}
    DreamCore
    -lpthread
    -ldl
    ${BULLET_LIBRARY}
    ${SDL2_LIBRARY}
    ${LUA_LIBRARIES}
)

if (WIN32)
	target_link_libraries(
	    ${PROJECT_NAME}
	    DreamCore
	    ${BULLET_LIBRARY}
	    ${SDL2_LIBRARY}
	    ${LUA_LIBRARIES}
	)
else()
	target_link_libraries(
	    ${PROJECT_NAME}
	    DreamCore
	    -lpthread
	    -ldl
	    ${BULLET_LIBRARY}
	    ${SDL2_LIBRARY}
	    ${LUA_LIBRARIES}
	)
endif()
