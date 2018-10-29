cmake_minimum_required (VERSION 2.8.0)
project(DreamToolImGui)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(ASSIMP     REQUIRED)
find_package(BULLET     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(GLM        REQUIRED)
find_package(Lua        REQUIRED)
find_package(OPENGL     REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
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
        ${GLFW_LIBRARY}
        ${LUA_LIBRARIES}
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
