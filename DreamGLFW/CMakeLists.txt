cmake_minimum_required (VERSION 2.8.0)
project(DreamGLFW CXX)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(BULLET     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(GLM        REQUIRED)
find_package(GLEW       REQUIRED)
find_package(assimp     REQUIRED)
find_package(Lua        REQUIRED)
find_package(FREETYPE   REQUIRED)
find_package(SOIL       REQUIRED)

include_directories(
    ${SOIL_INCLUDE_DIRS}
    ${DreamCore_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${assimp_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR_ft2build}
    ${SOIL_INCLUDE_DIR}
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
    ${SOIL_LIB}
    ${GLEW_LIBRARIES}
    ${assimp_LIBRARIES}
    ${BULLET_LIBRARY}
    ${GLFW_LIBRARY}
    ${LUA_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)
