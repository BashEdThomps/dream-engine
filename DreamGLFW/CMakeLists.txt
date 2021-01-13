cmake_minimum_required (VERSION 3.0)
project(DreamGLFW)


# Targets #####################################################################

add_executable (
    ${PROJECT_NAME}
	Window/GLFWWindowComponent.cpp
	Main.cpp
)

include_directories(${DreamCore_SOURCE_DIR}/include)


if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
    )
elseif(UNIX AND NOT APPLE) # Linux
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
        -lpthread
        -ldl
    )
elseif(APPLE)
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
		glfw
        -lpthread
        -ldl
        "-framework IOKit"
	"-framework AppKit"
	"-framework OpenGL"
	"-framework CoreFoundation"
	"-framework Carbon"
    )
endif()
