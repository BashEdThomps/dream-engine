cmake_minimum_required (VERSION 2.8.0)
project(DreamGLFW)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(ASSIMP     REQUIRED)
find_package(BULLET     REQUIRED)
find_package(GLEW       REQUIRED)
find_package(GLFW       REQUIRED)
find_package(GLM        REQUIRED)
find_package(Lua        REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(SPDLOG     REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}
)

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
                ${ASSIMP_LIBRARIES}
                ${BULLET_LIBRARY}
                ${GLEW_LIBRARY}
                ${GLFW_LIBRARY}
        ${LUA_LIBRARIES}
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
        -lpthread
        -ldl
            ${ASSIMP_LIBRARIES}
            ${BULLET_LIBRARY}
            ${GLEW_LIBRARY}
        ${GLFW_LIBRARY}
        ${LUA_LIBRARIES}
    )
endif()
