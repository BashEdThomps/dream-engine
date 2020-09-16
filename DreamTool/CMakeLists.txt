cmake_minimum_required (VERSION 3.0)
project(DreamTool)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(ASSIMP     REQUIRED)
find_package(BULLET     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(SPDLOG     REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}

	${DEP_GLAD_INC_DIR}
	${DEP_GLM_INC_DIR}
	${DEP_JSON_INC_DIR}
	${DEP_IMGUI_INC_DIR}
	${DEP_IMGUIZMO_INC_DIR}
	${DEP_IMTEXTEDITOR_INC_DIR}
	${DEP_IMFILESYSTEM_INC_DIR}
)

add_executable (
    ${PROJECT_NAME}
    ${SRC}

	${DEP_ANGELSCRIPT_INC_DIR}
	${DEP_IMFILESYSTEM_SRC_FILES}
	${DEP_IMGUI_SRC_FILES}
	${DEP_IMGUIZMO_SRC_FILES}
	${DEP_IMTEXTEDITOR_SRC_FILES}
)

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
        ${ASSIMP_LIBRARY_RELEASE}
        ${BULLET_LIBRARY}
        ${GLFW_LIBRARY}
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
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
