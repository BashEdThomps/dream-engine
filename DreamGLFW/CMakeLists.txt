cmake_minimum_required (VERSION 3.0)
project(DreamGLFW)

file(GLOB_RECURSE SRC "*.h" "*.cpp")

find_package(ASSIMP     REQUIRED)
find_package(BULLET     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(OPENGL     REQUIRED)
find_package(SPDLOG     REQUIRED)

include_directories(
    ${DreamCore_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}

	${DEP_ANGELSCRIPT_INC_DIR}
	${DEP_GLAD_INC_DIR}
	${DEP_GLM_INC_DIR}
	${DEP_JSON_INC_DIR}
    ${DEP_SPDLOG_INC_DIR}

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
    )
elseif(UNIX AND NOT APPLE) # Linux
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
        -lpthread
        -ldl
        ${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARY}
        ${GLFW_LIBRARY}
    )
elseif(APPLE)
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
