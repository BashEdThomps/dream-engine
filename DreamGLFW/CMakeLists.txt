cmake_minimum_required (VERSION 3.0)
project(DreamGLFW)

include_directories(${DreamCore_SOURCE_DIR}/include)

# Targets #####################################################################

add_executable (
    ${PROJECT_NAME}
	GLFWWindowComponent.cpp
	Main.cpp
)

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
