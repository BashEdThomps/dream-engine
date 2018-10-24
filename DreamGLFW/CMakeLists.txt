cmake_minimum_required (VERSION 2.8.0)
project(DreamGLFW CXX)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(BULLET     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(GLM        REQUIRED)
find_package(Lua        REQUIRED)
find_package(assimp     REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
	${assimp_INCLUDE_DIRS}
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}
)

if (WIN32)
	target_link_libraries(
		${PROJECT_NAME}
		DreamCore
		${BULLET_LIBRARY}
		${GLFW_LIBRARY}
		${LUA_LIBRARIES}
		${assimp_INCLUDE_DIRS}
	)
else()
	target_link_libraries(
	    ${PROJECT_NAME}
	    DreamCore
	    -lpthread
	    -ldl
	    ${BULLET_LIBRARY}
	    ${GLFW_LIBRARY}
	    ${LUA_LIBRARIES}
		${assimp_INCLUDE_DIRS}
	)
endif()
